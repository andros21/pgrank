/*
 * @project pgrank
 * @authors Andrea Rossoni
 * @version 0.1
 * @file    pagerank.hpp
 * @scope   Class library to compute pagerank algo
 *
 *  *--------------------------------------------*
 *  | WARNING: json file must be something like: |
 *  |                                            |
 *  |   {                                        |
 *  |      "master_key":                         |
 *  |         [                                  |
 *  |            {                               |
 *  |               "node_key": "node1",         |
 *  |               "nn_key": ["node3", "..."]   |
 *  |            },                              |
 *  |            {                               |
 *  |               "node_key": "node3",         |
 *  |               "nn_key": []                 |
 *  |            },                              |
 *  |            {                               |
 *  |               ...                          |
 *  |               ...                          |
 *  |            },                              |
 *  |         ]                                  |
 *  |   }                                        |
 *  |                                            |
 *  | So at "master_key" a list of dict, each    |
 *  | with a node name (type: string) at         |
 *  | "node_key" and its first neighbors nodes   |
 *  | (type: list string) at "nn_key"            |
 *  |                                            |
 *  | Also, all nn_nodes themselves must appear  |
 *  | as nodes, at most they will not present    |
 *  | any links (dangling nodes) but they must   |
 *  | be present to make the graph consistent    |
 *  *--------------------------------------------*
 *
 */

#include <json2mat.hpp>

#ifndef __PageRank__
#define __PageRank__

#define ALPHA 0.85   // Default dumping factor
#define EPSILON 1e-7 // Default epsilon

class PageRank
{
 private:
   double _alpha;      // Dumping factor
   arma::sp_mat _am;   // Adjacency matrix
   arma::rowvec _dang; // Dangling nodes vector
   arma::rowvec _rank; // Rank result vector

 public:
   /*
    * @scope Constructor of PageRank object from adjacency matrix
    * @param <am>    Armadillo spare adjacency matrix
    * @param <alpha> Dumping factor, default: ALPHA
    */
   PageRank(const arma::sp_mat&, const bool& = false, const double& = ALPHA);

   /*
    * @scope Constructor of PageRank object from normal adjacency
    *        matrix and dangling nodes vector
    * @param <pr>    Pair armadillo spare adjacency matrixa and
    *                dangling vector
    * @param <alpha> Dumping factor, default: ALPHA
    */
   PageRank(const std::pair<arma::sp_mat, arma::rowvec>&,
            const double& = ALPHA);

   /*
    * @scope Constructor of PageRank object from json
    * @param <json_path>  The input json filename
    * @param <info>       Stdout informations, default: false
    * @param <alpha>      Dumping factor, default: ALPHA
    * @param <master_key> Master key, default: "objects"
    * @param <node_key>   Node key, default: "url"
    * @param <nn_key>     Neighbors node key, default: "urls"
    */
   PageRank(const char*,
            const bool& = false,
            const double& = ALPHA,
            const char* = "objects",
            const char* = "url",
            const char* = "urls");

   /*
    * @scope Default nil destroyer
    */
   ~PageRank();

   /*
    * @scope Set dumping factor (_alpha)
    * @param <alpha> Dumping factor
    */
   void setAlpha(const double& alpha);

   /*
    * @scope  Get current dumping factor
    * @return (_alpha) current dumping factor
    *
    */
   double getAlpha();

   /*
    * @scope  Get arma adjacency matrix
    * @return Armadillo adjacency matrix (_am)
    *
    */
   arma::sp_mat getAdjMatrix();

   /*
    * @scope  Get arma dangling nodes vector
    * @return Armadillo dangling vector (_dang)
    *
    */
   arma::vec getDangVec();

   /*
    * @scope  Get arma rank result vector
    * @return Armadillo rank result vector (_rank)
    */
   arma::vec getRank();

   /*
    * @scope Write to csv rank result vector (_rank)
    * @param <out_path> Output path to save results
    * @param <info>     Stdout informations, default: false
    */
   void csvRank(const char*, const bool& = false);

   /*
    * @scope Evolve pagerank of _am with _alpha,
    *        and store result in _rank
    * @param <epsilon> Epsilon convergence error, default: EPSILON
    * @param <info>    Stdout informations, default: false
    */
   void evoRank(const double& = EPSILON, const bool& = false);

   /*
    * @scope Evolve pagerank of _sm with _alpha,
    *        and store result in _rank
    * @param <alpha>   Dumping factor
    * @param <epsilon> Epsilon convergence error, default: EPSILON
    * @param <info>    Stdout informations, default: false
    */
   void evoRank(const double&, const double& = EPSILON, const bool& = false);
};

#endif // ifndef __PageRank__
