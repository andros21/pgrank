/**
 * @authors Andrea Rossoni
 * @version 0.1.0
 *
 */

#include <pagerank.hpp>

// Constructor of PageRank object from adjacency matrix
PageRank::PageRank(const arma::sp_mat& am,
                   const bool& info,
                   const double& alpha)
   : _am(am)
   , _dang(_am.n_cols, arma::fill::zeros)
   , _rank(_am.n_cols)
{
   setAlpha(alpha);
   if (_am.n_cols != _am.n_rows)
      throw std::string(
         "(PageRank::PageRank) Invalid adjacency matrix, not square");
   if (info)
      fprintf(stdout,
              "[INFO] Normalize adjacency matrix, check dangling nodes, init "
              "rank vector\n");
   auto start = std::chrono::high_resolution_clock::now();
   // Normalize adjacency matrix, and populate dangling nodes vector
   for (long long unsigned int i = 0; i < _am.n_cols; ++i) {
      double accu = arma::accu(_am.row(i));
      if (accu != 0)
         _am.row(i) /= accu;
      else
         _dang(i) = 1.;
   }
   _rank.fill((double)1. / _am.n_cols);
   auto stop = std::chrono::high_resolution_clock::now();
   auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
   if (info)
      fprintf(stdout,
              "[INFO] Time: %lu\u00B5s, Size: %llu, Dangling nodes: %u\n\n",
              duration.count(),
              _rank.n_cols,
              (unsigned int)arma::accu(_dang));
}

// Constructor of PageRank object from normal adj matrix and dang vector
PageRank::PageRank(const std::pair<arma::sp_mat, arma::rowvec>& pr,
                   const double& alpha)
   : _am(pr.first)
   , _dang(pr.second)
   , _rank(_am.n_cols)
{
   setAlpha(alpha);
   if (_am.n_cols != _am.n_rows)
      throw std::string(
         "(PageRank::PageRank) Invalid adjacency matrix, not square");
   if (_am.n_cols != _dang.n_cols)
      throw std::string("(PageRank::PageRank) Invalid dangling vector, not "
                        "same size of adjacency matrix");
   _rank.fill((double)1. / _am.n_cols);
}

// Constructor of PageRank object from json
PageRank::PageRank(const char* json_path,
                   const bool& info,
                   const double& alpha,
                   const char* master_key,
                   const char* node_key,
                   const char* nn_key)
   : PageRank(
        adjacencyMatrixJson(json_path, info, master_key, node_key, nn_key),
        alpha)
{}

// Default nil destroyer
PageRank::~PageRank() {}

// Set dumping factor (_alpha)
void
PageRank::setAlpha(const double& alpha)
{
   if (alpha < 0. || alpha > 1.)
      throw std::string("(PageRank::setAlpha) Dumping factor <") +
         std::to_string(alpha) +
         std::string("> invalid, \u03B1 must be \u03B1 \u2208 [0,1]");
   _alpha = alpha;
}

// Get current dumping factor
double
PageRank::getAlpha()
{
   return _alpha;
}

// Get arma adjacency matrix
arma::sp_mat
PageRank::getAdjMatrix()
{
   return _am;
}

// Get arma dangling vector
arma::vec
PageRank::getDangVec()
{
   return _dang.t();
}

// Get arma rank result vector
arma::vec
PageRank::getRank()
{
   return _rank.t();
}

// Write to csv rank result vector (_rank)
void
PageRank::csvRank(const char* out_path, const bool& info)
{
   if (info)
      fprintf(
         stdout, "[INFO] Save rank result vector to <%s> as csv\n", out_path);
   auto start = std::chrono::high_resolution_clock::now();
   arma::vec rank = _rank.t();
   rank.save(out_path, arma::csv_ascii);
   auto stop = std::chrono::high_resolution_clock::now();
   auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
   if (info)
      fprintf(stdout,
              "[INFO] Time: %lu\u00B5s, Size: %llu\n\n",
              duration.count(),
              _rank.n_cols);
}

// Evolve pagerank of _am with _alpha, and store result in _rank
void
PageRank::evoRank(const double& epsilon, const bool& info)
{
   if (info)
      fprintf(stdout, "[INFO] Evolve rank vector using power iteration\n");
   auto start = std::chrono::high_resolution_clock::now();
   unsigned int it(0);
   auto N = _am.n_cols;
   double wdang;
   double a = _alpha / N;
   double b = (1. - _alpha) / N;
   arma::rowvec rank(N);
   arma::rowvec ones(N, arma::fill::ones);
   do {
      rank = _rank;
      wdang = arma::dot(rank, _dang);
      _rank = rank * _am * _alpha + ones * a * wdang + ones * b;
      ++it;
   } while (arma::norm(_rank - rank) >= epsilon);
   auto stop = std::chrono::high_resolution_clock::now();
   auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
   if (info)
      fprintf(
         stdout,
         "[INFO] Time: %lu\u00B5s, Size: %llu, Dumping factor: %f, Iterations: "
         "%u\n\n",
         duration.count(),
         _am.n_cols,
         _alpha,
         it);
}

// Evolve pagerank of _am with _alpha, and store result in _rank
void
PageRank::evoRank(const double& alpha, const double& epsilon, const bool& info)
{
   setAlpha(alpha);
   evoRank(epsilon, info);
}
