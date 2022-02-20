/**
 * @authors Andrea Rossoni
 * @version 0.1.1
 *
 */

#include <json2mat.hpp>

// From json file to rapidjson document object
rapidjson::Document
_parseJson(const char* json_path)
{
   std::ifstream ifs(json_path);
   if (!ifs)
      throw std::string("(_parseJson) Invalid file <") + json_path +
         std::string(">");
   rapidjson::IStreamWrapper isw(ifs);
   rapidjson::Document d;
   rapidjson::ParseResult res = d.ParseStream(isw);
   if (!res)
      throw std::string("(_parseJson) Invalid json <") + json_path +
         std::string(">");
   return d;
}

// From json file to unordered_map (string->uint)
std::unordered_map<std::string, unsigned int>
_uMapJson(const char* json_path,
          const bool& info,
          const char* master_key,
          const char* node_key)
{
   unsigned int i(0);
   auto d = _parseJson(json_path);
   std::unordered_map<std::string, unsigned int> ht;
   if (!d.HasMember(master_key))
      throw std::string("(_uMapJson) Invalid master_key <") + master_key +
         std::string(">") + std::string(" inside <") + json_path +
         std::string(">");
   if (!d[master_key].IsArray())
      throw std::string("(_uMapJson) No array found at <") + master_key +
         std::string(">") + std::string(" inside <") + json_path +
         std::string(">");
   if (info)
      fprintf(stdout,
              "[INFO] Load node(url) inside hash table (string->uint)\n");
   auto start = std::chrono::high_resolution_clock::now();
   for (auto& v : d[master_key].GetArray()) {
      if (!v.IsObject())
         throw std::string("(_uMapJson) No object found in array at <") +
            master_key + std::string("><") + node_key +
            std::string("> inside <") + json_path + std::string(">");
      if (!v.HasMember(node_key))
         throw std::string("(_uMapJson) Invalid node_key <") + master_key +
            std::string("><") + node_key + std::string("> inside <") +
            json_path + std::string(">");
      if (!v[node_key].IsString())
         throw std::string("(_uMapJson) No string found at <") + master_key +
            std::string("><") + node_key + std::string("> inside <") +
            json_path + std::string(">");
      ht[v[node_key].GetString()] = i;
      ++i;
   }
   auto stop = std::chrono::high_resolution_clock::now();
   auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
   if (info)
      fprintf(stdout,
              "[INFO] Time: %lu\u00B5s, Size: %lu, Bucket count: %lu, Load "
              "factor: %f\n\n",
              duration.count(),
              ht.size(),
              ht.bucket_count(),
              ht.load_factor());
   return ht;
}

// Create normal adjacency matrix and dangling vector
std::pair<arma::sp_mat, arma::rowvec>
adjacencyMatrixJson(const char* json_path,
                    const bool& info,
                    const char* master_key,
                    const char* node_key,
                    const char* nn_key)
{
   auto d = _parseJson(json_path);
   auto ht = _uMapJson(json_path, info, master_key, node_key);
   arma::sp_mat am(ht.size(), ht.size());
   arma::rowvec dang(ht.size(), arma::fill::zeros);
   if (info)
      fprintf(stdout,
              "[INFO] Create adjacency matrix and dangling vector from <%s>\n",
              json_path);
   auto start = std::chrono::high_resolution_clock::now();
   for (auto& v : d[master_key].GetArray()) {
      if (!v.HasMember(nn_key))
         throw std::string("(adjacencyMatrixJson) Invalid nn_key <") +
            master_key + std::string("><") + node_key +
            std::string("> inside <") + json_path + std::string(">");
      if (!v[nn_key].IsArray())
         throw std::string("(adjacencyMatrixJson) No array found at <") +
            master_key + std::string("><") + nn_key +
            std::string("> inside <") + json_path + std::string(">");
      std::string node_url = v[node_key].GetString(); // Normalization factor
      unsigned int nn_size =
         v[nn_key].GetArray().Size(); // Check dangling nodes
      if (nn_size == 0)
         dang(ht[node_url]) = 1.;
      for (auto& w : v[nn_key].GetArray()) {
         if (!w.IsString())
            throw std::string(
               "(adjacencyMatrixJson) No string found in array at <") +
               master_key + std::string("><") + nn_key +
               std::string("> inside <") + json_path + std::string(">");
         auto nn_url = w.GetString();
         if (ht.contains(node_url) && ht.contains(nn_url))
            am(ht[node_url], ht[nn_url]) = (double)1. / nn_size;
         else
            throw std::string("(adjacencyMatrixJson) No node <") + nn_url +
               std::string("> found in hash table generated from <") +
               json_path + std::string(">, maybe not explored");
      }
   }
   auto stop = std::chrono::high_resolution_clock::now();
   auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
   if (info)
      fprintf(stdout,
              "[INFO] Time: %lu\u00B5s, Size: %llux%llu, Non zero: %llu, "
              "Dangling: %llu\n\n",
              duration.count(),
              am.n_rows,
              am.n_cols,
              am.n_nonzero,
              (long long unsigned int)arma::accu(dang));
   return { am, dang };
}
