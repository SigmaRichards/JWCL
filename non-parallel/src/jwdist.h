/**@file
 * @brief Jaro-Winkler string similarity implementation
 * @author Stefan Hamminga <s@stefanhamminga.com>
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <string_view>





namespace edit_distance {
    template <typename T = float>
    inline T jaro(const std::string_view source,
                  const std::string_view target)
    {
      const unsigned sl = source.length();
      const unsigned tl = target.length();

      if (sl == 0 || tl == 0) return 0;
      // if (sl == 0) {
      //     return tl == 0 ? 1 : 0;
      // }
      const auto match_distance = (sl == 1 && tl == 1)
                                      ? 0
                                      : (std::max(sl, tl) / 2 - 1);
      auto source_matches = new bool[sl] {0};
      auto target_matches = new bool[tl] {0};
      unsigned matches = 0;
      for (unsigned i = 0; i < sl; ++i) {
          const auto end = std::min(i + match_distance + 1, tl);
          for (auto k = i > match_distance ? (i - match_distance) : 0u; k < end; ++k)
          {
              if (!target_matches[k] && source[i] == target[k]) {
                  source_matches[i] = true;
                  target_matches[k] = true;
                  ++matches;
                  break;
              }
          }
      }
      if (matches == 0) {
          delete[] source_matches;
          delete[] target_matches;
          return 0;
      }

      T t = 0.0;
      unsigned k = 0;
      for (unsigned i = 0; i < sl; ++i) {
          if (source_matches[i]) {
              while (!target_matches[k]) ++k;
              if (source[i] != target[k]) t += 0.5;
              ++k;
          }
      }

      const T m = matches;
      delete[] source_matches;
      delete[] target_matches;
      return (m / sl + m / tl + (m - t) / m) / 3.0;
    }

    template <typename T = float>
    inline T jaro_winkler(const std::string_view source,
                          const std::string_view target,
                          const unsigned prefix = 4,
                          const T boost_treshold = 0.7,
                          const T scaling_factor = 0.1)
    {
        const auto similarity = jaro<T>(source, target);

        if (similarity > boost_treshold) {
            int common_prefix = 0;
            const int l = std::min((unsigned)std::min(source.length(), target.length()), prefix);
            for (; common_prefix < l; ++common_prefix) {
                if (source[common_prefix] != target[common_prefix]) break;
            }
            return similarity + scaling_factor * common_prefix * (1 - similarity);
        } else {
            return similarity;
        }
    }
} // namespace edit_distance


std::vector<double> jw_self_outer(std::vector<std::string_view> stringlist){
  std::vector<double> out;
  double dis = 0;
  int n_strings = stringlist.size();
  for(int i = 0; i < n_strings ; i++){
    for(int j = 0; j < n_strings ; j++){
      dis = edit_distance::jaro_winkler(stringlist[i],stringlist[j]);
      out.push_back(dis);
    }
  }

  return out;
}
