/*
 * @project pgrank
 * @authors Andrea Rossoni
 * @file    pgrank.cpp
 * @scope   Calculate rank and save csv results starting from json
 */

#include <argparse/argparse.hpp>
#include <pagerank.hpp>

int
main(int argc, char* argv[])
{
   argparse::ArgumentParser program("pgrank", "0.1.1");
   program.add_argument("input").help("input json file");
   program.add_argument("output").help("output csv file");
   program.add_argument("-a", "--alpha")
      .help("dumping factor")
      .default_value(ALPHA)
      .action([](const std::string& value) { return std::stod(value); });
   program.add_argument("-e", "--epsilon")
      .help("epsilon convergence error")
      .default_value(EPSILON)
      .action([](const std::string& value) { return std::stod(value); });
   program.add_argument("-m", "--master")
      .help("master key of json")
      .default_value(std::string("objects"))
      .action([](const std::string& value) { return value; });
   program.add_argument("-n", "--node")
      .help("node key of json")
      .default_value(std::string("url"));
   program.add_argument("-nn", "--nnode")
      .help("neighbors node key of json")
      .default_value(std::string("urls"));
   program.add_argument("-q", "--quiet")
      .help("no stdout info")
      .default_value(false)
      .implicit_value(true);
   try {
      program.parse_args(argc, argv);
      PageRank pgrank(program.get<std::string>("input").c_str(),
                      !program.get<bool>("--quiet"),
                      program.get<double>("--alpha"),
                      program.get<std::string>("--master").c_str(),
                      program.get<std::string>("--node").c_str(),
                      program.get<std::string>("--nnode").c_str());
      pgrank.evoRank(program.get<double>("--epsilon"),
                     !program.get<bool>("--quiet"));
      std::ofstream ofs(program.get<std::string>("output").c_str());
      if (!ofs)
         throw std::string("(main) Invalid output path <") +
            program.get<std::string>("output") + std::string(">");
      pgrank.csvRank(program.get<std::string>("output").c_str(),
                     !program.get<bool>("--quiet"));
   } catch (const std::string& msg) {
      fprintf(stderr, "[ERROR] %s\n", msg.c_str());
      return EXIT_FAILURE;
   } catch (const std::runtime_error& err) {
      fprintf(stderr, "[ERROR] %s\n", err.what());
      return EXIT_FAILURE;
   }
   return EXIT_SUCCESS;
}
