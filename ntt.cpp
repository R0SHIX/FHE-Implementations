#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
extern "C" {
double walltime_();
double cputime_();
}
using ll = int64_t;

//---------------------------------------------------------------------------------------
//                          Functions
//---------------------------------------------------------------------------------------

// This function exists to stop negative numbers from appearing
ll mod(ll a, ll m) { return ((a % m) + m) % m; }

// Exists to keep numbers from exploding into large values
ll modpow(ll base, ll exp, ll m) {

  ll result = 1;

  while (exp > 0) {

    if (exp & 1) {
      result = (result * base) % m;
    }
    base = (base * base) % m;
    exp = exp >> 1;
  }
  return result;
}

void bitreversal(std::vector<ll> &a) {
  int bits = std::log2(a.size());

  for (size_t i = 0; i < a.size(); i++) {
    int j = 0;
    for (int k = 0; k < bits; k++) {
      j = (j << 1) | ((i >> k) & 1);
    }
    if (j > i) {
      std::swap(a[i], a[j]);
    }
  }
}

void ntt(std::vector<ll> &a, ll q, ll omega) {
  bitreversal(a);
  int stages = (int)log2(a.size());

  for (size_t stage = 0; stage < stages; stage++) {
    ll half = (1 << stage);

    std::vector<ll> twiddles(half);
    for (ll k = 0; k < half; k++) {
      twiddles[k] = modpow(omega, k * ((ll)a.size() / (2 * half)), q);
    }

    for (size_t group = 0; group < a.size(); group += 2 * half) {
      for (size_t butterfly = group; butterfly < group + half; butterfly++) {
        ll exp = (ll)(butterfly - group) * ((ll)a.size() / (ll)(2 * half));
        ll twiddle_factor = twiddles[butterfly - group];

        ll upper = a[butterfly];
        ll lower = mod(a[butterfly + half] * twiddle_factor, q);
        a[butterfly] = mod(upper + lower, q);
        a[butterfly + half] = mod(upper - lower + q, q);
      }
    }
  }
}

void intt(std::vector<ll> &a, ll q, ll omega) {
  ll omega_inv = modpow(omega, q - 2, q);
  ll n_inv = modpow(a.size(), q - 2, q);
  ntt(a, q, omega_inv);
  for (size_t i = 0; i < a.size(); i++) {
    a[i] = mod(a[i] * n_inv, q);
  }
}

std::vector<ll> poly_multiply(std::vector<ll> &f, std::vector<ll> &g, ll q,
                              ll omega) {
  int n = f.size() + g.size() - 1; // minimum size
  int padded = 1;
  while (padded < n)
    padded <<= 1; // next power of 2
  std::vector<ll> tmp_f(padded, 0);
  std::vector<ll> tmp_g(padded, 0);
  for (size_t i = 0; i < f.size(); i++)
    tmp_f[i] = f[i];
  for (size_t i = 0; i < g.size(); i++)
    tmp_g[i] = g[i];

  ntt(tmp_f, q, omega);

  ntt(tmp_g, q, omega);

  std::vector<ll> result(padded);

  for (size_t i = 0; i < padded; i++) {
    result[i] = mod(tmp_f[i] * tmp_g[i], q);
  }

  intt(result, q, omega);
  return result;
}

std::vector<ll> load_poly(const std::string &filename) {
  std::ifstream file(filename);
  std::string line;
  std::getline(file, line);
  std::istringstream iss(line);
  std::vector<ll> poly;
  ll val;
  while (iss >> val)
    poly.push_back(val);
  return poly;
}
int main(int argc, char *argv[]) {
  //---------------------------------------------------------------------------------------
  //                    Test Values
  //---------------------------------------------------------------------------------------
  ll q = atoll(argv[1]), omega = atoll(argv[2]);
  std::vector<ll> f = load_poly(argv[3]);
  std::vector<ll> g = load_poly(argv[4]);
  auto expected = load_poly(argv[5]);

  //---------------------------------------------------------------------------------------
  //                    Timing variables Init
  //---------------------------------------------------------------------------------------
  double wall = walltime_();
  double cpu = cputime_();

  auto result = poly_multiply(f, g, q, omega);
  //---------------------------------------------------------------------------------------
  //                    Timing variables End
  //---------------------------------------------------------------------------------------
  wall = walltime_() - wall;
  cpu = cputime_() - cpu;

  std::cout << "result size: " << result.size() << std::endl;
  std::cout << "expected size: " << expected.size() << std::endl;
  std::cout << "RUNTIME: " << cpu << " seconds " << std::endl;
  // verify
  bool pass = (result == expected);
  std::cout << (pass ? "PASS" : "FAIL") << std::endl;
  if (!pass) {
    for (int i = 0; i < 4; i++)
      std::cout << "got " << result[i] << " expected " << expected[i] << "\n";
  }
  return 0;
}
