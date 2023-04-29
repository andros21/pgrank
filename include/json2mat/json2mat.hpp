/**
 * @file
 * @authors Andrea Rossoni
 * @version 0.1.5
 *
 * @subsubsection Description
 *
 * Library to parse json files and convert,
 * them into armadillo adjacency matrix
 *
 * **WARNING** json file must be something like:
 *
 * ```
 *   {
 *      "master_key":
 *         [
 *            {
 *               "node_key": "node1",
 *               "nn_key": ["node3", "..."]
 *            },
 *            {
 *               "node_key": "node3",
 *               "nn_key": []
 *            },
 *            {
 *               ...
 *               ...
 *            },
 *         ]
 *   }
 * ```
 *
 * So at **master_key** a list of dict, each
 * with a node name (type: string) at
 * **node_key** and its first neighbors nodes
 * (type: list string) at **nn_key**
 *
 * Also, all nn_nodes themselves must appear
 * as nodes, at most they will not present
 * any links (dangling nodes) but they must
 * be present to make the graph consistent
 *
 */

#define ARMA_DONT_USE_WRAPPER // No wrap, linking with openblas
#include <armadillo>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

/**
 * @brief  From json file to rapidjson document object
 * @param  json_path The input json filename
 * @return Rapidjson document object containing parsed json
 */
rapidjson::Document
_parseJson(const char*);

/**
 * @brief  From json file to unordered_map (string->uint),
 *         mapping needed to populate adjacency matrix
 * @param  json_path  The input json filename
 * @param  info       Stdout informations, default: **false**
 * @param  master_key Master key, default: **objects**
 * @param  node_key   Node key, default: **url**
 * @return Unordered map mapping node names from json to unsigned int
 */
std::unordered_map<std::string, unsigned int>
_uMapJson(const char*,
          const bool& = false,
          const char* = "objects",
          const char* = "url");

/**
 * @brief  Create normal adjacency matrix and dangling vector
 *         from json file
 * @param  json_path  The input json filename
 * @param  info       Stdout informations, default: **false**
 * @param  master_key Master key, default: **objects**
 * @param  node_key   Node key, default: **url**
 * @param  nn_key     Neighbors node key, default: **urls**
 * @return Adjacency matrix as armadillo spare matrix and
 *         dangling vector as armadillo rowvector
 */
std::pair<arma::sp_mat, arma::rowvec>
adjacencyMatrixJson(const char*,
                    const bool& = false,
                    const char* = "objects",
                    const char* = "url",
                    const char* = "urls");
