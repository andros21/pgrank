/*
 * @project pgrank
 * @authors Andrea Rossoni
 * @version 0.1
 * @file    pgrank_test.cpp
 * @scope   Catch2 unit tests for pgrank
 *
 */

#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this
                          // in one cpp file
#include <catch2/catch.hpp>
#include <pagerank.hpp>

TEST_CASE("Check json input file", "[json_input]")
{
   CHECK_THROWS_WITH(_parseJson("../tests/input/an_invalid_file"),
                     Catch::Matchers::Contains("Invalid file"));
   CHECK_THROWS_WITH(_parseJson("../tests/input/no_json_here.txt"),
                     Catch::Matchers::Contains("Invalid json"));
   CHECK_THROWS_WITH(_parseJson("../tests/input/invalid.json"),
                     Catch::Matchers::Contains("Invalid json"));
}
TEST_CASE("Check json structure", "[json_struct]")
{
   CHECK_THROWS_WITH(_uMapJson("../tests/input/valid.no-keys.json"),
                     Catch::Matchers::Contains("Invalid master_key"));
   CHECK_THROWS_WITH(
      _uMapJson("../tests/input/valid.no-keys.json", false, "dicts"),
      Catch::Matchers::Contains("Invalid node_key"));
   CHECK_THROWS_WITH(
      adjacencyMatrixJson(
         "../tests/input/valid.no-keys.json", false, "dicts", "node"),
      Catch::Matchers::Contains("Invalid nn_key"));

   CHECK_THROWS_WITH(_uMapJson("../tests/input/valid.no-arr.json"),
                     Catch::Matchers::Contains("No array"));
   CHECK_THROWS_WITH(_uMapJson("../tests/input/valid.no-arr-obj.json"),
                     Catch::Matchers::Contains("No object found in array"));
   CHECK_THROWS_WITH(_uMapJson("../tests/input/valid.no-str.json"),
                     Catch::Matchers::Contains("No string"));

   CHECK_THROWS_WITH(adjacencyMatrixJson("../tests/input/valid.no-nnarr.json"),
                     Catch::Matchers::Contains("No array"));
   CHECK_THROWS_WITH(
      adjacencyMatrixJson("../tests/input/valid.no-nnarr-str.json"),
      Catch::Matchers::Contains("No string found in array"));
   CHECK_THROWS_WITH(adjacencyMatrixJson("../tests/input/valid.node-lack.json"),
                     Catch::Matchers::Contains("No node") &&
                        Catch ::Matchers::Contains("found"));
}
TEST_CASE("Check json2mat lib", "[json2mat]")
{
   std::string input("../tests/input/caibg.fake.json");
   REQUIRE_NOTHROW(adjacencyMatrixJson(input.c_str(), true));

   auto doc = _parseJson(input.c_str());
   REQUIRE(doc["objects"].GetArray()[0]["url"].GetString() ==
           std::string(
              "http://geoportale.caibergamo.it/it/rifugio/baita-lago-nero"));
   REQUIRE(doc["objects"].GetArray()[3]["urls"].GetArray()[0].GetString() ==
           std::string("http://geoportale.caibergamo.it/it/rifugio/"
                       "rifugio-mario-merelli-al-coca"));
   REQUIRE(doc["objects"].GetArray()[6]["url"].GetString() ==
           std::string(
              "http://geoportale.caibergamo.it/it/rifugio/bivacco-zamboni"));

   auto umap = _uMapJson(input.c_str());
   REQUIRE(umap.contains(
              "http://geoportale.caibergamo.it/it/rifugio/baita-lago-nero") ==
           true);
   REQUIRE(umap.contains("http://geoportale.caibergamo.it/it/sentiero/215") ==
           false);

   auto pr = adjacencyMatrixJson(input.c_str());
   REQUIRE(pr.first(0, 0) == (double)0.);
   REQUIRE(pr.first(3, 4) == (double)1.);
   REQUIRE(pr.first(4, 3) == (double)1.);
   REQUIRE(pr.first(6, 6) == (double)0.);
   REQUIRE(pr.second(0) == (double)1.);
   REQUIRE(pr.second(3) == (double)0.);
}
TEST_CASE("Check pagerank lib", "[pagerank]")
{
   arma::sp_mat wm(5, 7);
   arma::sp_mat cm(7, 7);
   arma::rowvec wv(5);
   std::pair<arma::sp_mat, arma::rowvec> wp1 = { wm, wv };
   std::pair<arma::sp_mat, arma::rowvec> wp2 = { cm, wv };
   CHECK_THROWS_WITH(PageRank(wm),
                     Catch::Matchers::Contains("Invalid adjacency matrix") &&
                        Catch::Matchers::Contains("not square"));
   CHECK_THROWS_WITH(PageRank(wp1),
                     Catch::Matchers::Contains("Invalid adjacency matrix") &&
                        Catch::Matchers::Contains("not square"));
   CHECK_THROWS_WITH(PageRank(wp2),
                     Catch::Matchers::Contains("Invalid dangling vector") &&
                        Catch::Matchers::Contains("not same size"));

   std::string input("../tests/input/caibg.fake.json");
   auto pr = adjacencyMatrixJson(input.c_str());
   auto prc1 = PageRank(pr.first, true);
   auto prc2 = PageRank(input.c_str());
   arma::vec empty;
   arma::vec norm(pr.first.n_rows);
   norm.fill((double)1. / pr.first.n_rows);

   REQUIRE(prc1.getAlpha() == ALPHA);
   REQUIRE(prc1.getAlpha() == prc2.getAlpha());

   REQUIRE(arma::approx_equal(
      prc1.getAdjMatrix(), prc2.getAdjMatrix(), "absdiff", (double)1.e-15));

   REQUIRE(arma::approx_equal(
      prc2.getDangVec(), pr.second.t(), "absdiff", (double)1.e-15));
   REQUIRE(arma::approx_equal(
      prc1.getDangVec(), prc2.getDangVec(), "absdiff", (double)1.e-15));

   REQUIRE(arma::approx_equal(prc1.getRank(), norm, "absdiff", (double)1.e-15));
   REQUIRE(arma::approx_equal(
      prc1.getRank(), prc2.getRank(), "absdiff", (double)1.e-15));

   CHECK_NOTHROW(prc1.evoRank(0.81, EPSILON, true));
   CHECK_NOTHROW(prc2.setAlpha(0.81));

   REQUIRE(prc1.getAlpha() == (double)0.81);
   REQUIRE(prc1.getAlpha() == prc2.getAlpha());
   CHECK_THROWS_WITH(prc2.setAlpha(-0.99),
                     Catch::Matchers::Contains("Dumping factor") &&
                        Catch::Matchers::Contains("invalid"));
   CHECK_THROWS_WITH(prc2.setAlpha(1.01),
                     Catch::Matchers::Contains("Dumping factor") &&
                        Catch::Matchers::Contains("invalid"));

   REQUIRE_NOTHROW(prc1.csvRank("/tmp/caibg.fake.csv", true));
}
