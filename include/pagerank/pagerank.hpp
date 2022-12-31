/**
 * @file
 * @authors Andrea Rossoni
 * @version 0.1.4
 *
 * @subsubsection Description
 *
 * Class library to compute pagerank algo
 *
 * @see json2mat.hpp
 *
 */

#ifndef __PageRank__
#define __PageRank__

#include <json2mat.hpp>

#define ALPHA 0.85   ///< Default dumping factor
#define EPSILON 1e-7 ///< Default epsilon

/**
 * @brief PageRank class (for computing pagerank)
 */
class PageRank
{
 private:
   double _alpha;      // Dumping factor
   arma::sp_mat _am;   // Adjacency matrix
   arma::rowvec _dang; // Dangling nodes vector
   arma::rowvec _rank; // Rank result vector

 public:
   /**
    * @brief Constructor of PageRank object from adjacency matrix
    * @param am    Armadillo spare adjacency matrix
    * @param info  Stdout informations, default: **false**
    * @param alpha Dumping factor, default: **ALPHA**
    */
   PageRank(const arma::sp_mat&, const bool& = false, const double& = ALPHA);

   /**
    * @brief Constructor of PageRank object from normal adjacency
    *        matrix and dangling nodes vector
    * @param pr    Pair armadillo spare adjacency matrixa and
    *                dangling vector
    * @param alpha Dumping factor, default: **ALPHA**
    */
   PageRank(const std::pair<arma::sp_mat, arma::rowvec>&,
            const double& = ALPHA);

   /**
    * @brief Constructor of PageRank object from json
    * @param json_path  The input json filename
    * @param info       Stdout informations, default: **false**
    * @param alpha      Dumping factor, default: **ALPHA**
    * @param master_key Master key, default: **objects**
    * @param node_key   Node key, default: **url**
    * @param nn_key     Neighbors node key, default: **urls**
    */
   PageRank(const char*,
            const bool& = false,
            const double& = ALPHA,
            const char* = "objects",
            const char* = "url",
            const char* = "urls");

   /**
    * @brief Default nil destroyer
    */
   ~PageRank();

   /**
    * @brief Set dumping factor (_alpha)
    * @param alpha Dumping factor
    */
   void setAlpha(const double& alpha);

   /**
    * @brief  Get current dumping factor
    * @return (_alpha) current dumping factor
    *
    */
   double getAlpha();

   /**
    * @brief  Get arma adjacency matrix
    * @return Armadillo adjacency matrix (_am)
    *
    */
   arma::sp_mat getAdjMatrix();

   /**
    * @brief  Get arma dangling nodes vector
    * @return Armadillo dangling vector (_dang)
    *
    */
   arma::vec getDangVec();

   /**
    * @brief  Get arma rank result vector
    * @return Armadillo rank result vector (_rank)
    */
   arma::vec getRank();

   /**
    * @brief Write to csv rank result vector (_rank)
    * @param out_path Output path to save results
    * @param info     Stdout informations, default: **false**
    */
   void csvRank(const char*, const bool& = false);

   /**
    * @brief Evolve pagerank of _am with _alpha,
    *        and store result in _rank
    * @param epsilon Epsilon convergence error, default: **EPSILON**
    * @param info    Stdout informations, default: **false**
    */
   void evoRank(const double& = EPSILON, const bool& = false);

   /**
    * @brief Evolve pagerank of _sm with _alpha,
    *        and store result in _rank
    * @param alpha   Dumping factor
    * @param epsilon Epsilon convergence error, default: **EPSILON**
    * @param info    Stdout informations, default: **false**
    */
   void evoRank(const double&, const double& = EPSILON, const bool& = false);
};

#endif // ifndef __PageRank__
