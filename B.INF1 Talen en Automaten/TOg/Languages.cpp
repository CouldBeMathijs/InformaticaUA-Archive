#include "Languages.h"
#include "Norkella/ENFA.h"
#include "Norkella/General.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <future>
#include <mutex>
namespace Languages {
void exportToImage(const std::map<std::string, double>& data, const std::string& outputImage) {
  std::ofstream dataFile1("data_normal.txt");
  std::ofstream dataFile2("data_highlight.txt");
  if (!dataFile1 || !dataFile2) {
    std::cerr << "Error creating data files." << std::endl;
    return;
  }

  // Find the index of the highest value
  double maxValue = -1.0;
  int maxIndex = -1;
  int index = 0;
  for (const auto& pair : data) {
    if (pair.second > maxValue) {
      maxValue = pair.second;
      maxIndex = index;
    }
    ++index;
  }

  index = 0;
  std::vector<std::string> labels;
  for (const auto& pair : data) {
    if (index == maxIndex) {
      dataFile2 << index << " " << pair.second << std::endl;
      dataFile1 << index << " 0\n"; // placeholder to keep bar positions aligned
    } else {
      dataFile1 << index << " " << pair.second << std::endl;
      dataFile2 << index << " 0\n"; // placeholder
    }
    labels.push_back(pair.first);
    ++index;
  }

  dataFile1.close();
  dataFile2.close();

  std::ofstream gnuplotScript("plot_script.gp");
  if (!gnuplotScript) {
    std::cerr << "Error creating Gnuplot script." << std::endl;
    return;
  }

  gnuplotScript << "set terminal pngcairo size 800,600 enhanced font 'Arial,10'\n";
  gnuplotScript << "set output '" << outputImage << "'\n";
  gnuplotScript << "set object 1 rectangle from screen 0,0 to screen 1,1 fillcolor rgb '#282832' behind\n";
  gnuplotScript << "set border lc rgb 'white'\n";
  gnuplotScript << "set tics textcolor rgb 'white'\n";
  gnuplotScript << "set title 'Language results' textcolor rgb 'white'\n";
  gnuplotScript << "set xlabel 'Language' textcolor rgb 'white'\n";
  gnuplotScript << "set ylabel 'Token Matches' textcolor rgb 'white'\n";
  gnuplotScript << "set xtics rotate by -45 textcolor rgb 'white'\n";
  gnuplotScript << "set ytics textcolor rgb 'white'\n";
  gnuplotScript << "set boxwidth 0.5\n";
  gnuplotScript << "set style fill solid\n";
  gnuplotScript << "set xtics (";

  for (size_t i = 0; i < labels.size(); ++i) {
    gnuplotScript << "'" << labels[i] << "' " << i;
    if (i != labels.size() - 1)
      gnuplotScript << ", ";
  }
  gnuplotScript << ")\n";

  gnuplotScript << "plot 'data_normal.txt' using 1:2 with boxes lc rgb '#141414' title '', \\\n";
  gnuplotScript << "     'data_highlight.txt' using 1:2 with boxes lc rgb '#4fa3f7' title ''\n";

  gnuplotScript.close();

  if (std::system("gnuplot plot_script.gp") != 0) {
    std::cerr << "Gnuplot exited abnormally... continuing anyway" << std::endl;
  };

  std::remove("data_normal.txt");
  std::remove("data_highlight.txt");
  std::remove("plot_script.gp");
}


class ThreadLimiter {
public:
  explicit ThreadLimiter(size_t max) : maxThreads(max), activeThreads(0) {}

  void acquire() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [&] { return activeThreads < maxThreads; });
    ++activeThreads;
  }

  void release() {
    std::lock_guard<std::mutex> lock(mtx);
    --activeThreads;
    cv.notify_one();
  }

private:
  size_t maxThreads;
  size_t activeThreads;
  std::mutex mtx;
  std::condition_variable cv;
};

void generateImages(const std::string &extension = "svg") {
  namespace fs = std::filesystem;
  using namespace std::chrono;

  const fs::path languagesDir = "languages";
  const fs::path imagesDir = "images";

  if (!fs::exists(imagesDir)) {
    fs::create_directory(imagesDir);
  }

  size_t maxThreads = std::thread::hardware_concurrency();
  if (maxThreads == 0) maxThreads = 4; // Fallback

  ThreadLimiter limiter(maxThreads);
  std::vector<std::thread> threads;

  for (const auto& entry : fs::directory_iterator(languagesDir)) {
    if (entry.is_regular_file() && entry.path().extension() == ".json") {
      std::string languageName = entry.path().stem().string();
      fs::path outputImage = imagesDir / (languageName + "." + extension);

      if (fs::exists(outputImage)) {
        std::cout << "Image already exists for: " << languageName << std::endl;
        continue;
      }

      limiter.acquire();
      threads.emplace_back([entry, outputImage, languageName, &limiter,
                            extension]() {
        using namespace std::chrono;
        std::cout << "Now generating image for: " << languageName << std::endl;
        auto start = high_resolution_clock::now();
        try {
          Norkella::DFA dfa(entry.path().string());
          dfa.toImage(outputImage.string(), extension);
        } catch (const std::exception& e) {
          std::cerr << "Error generating image for " << languageName << ": " << e.what() << std::endl;
        }
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);
        std::cout << "Finished in " << duration.count() << " ms\n";

        limiter.release();
      });
    }
  }

  for (auto& t : threads) {
    if (t.joinable()) {
      t.join();
    }
  }
}

void generateAll() {
  generateLanguages();
  generateImages("svg");
  generateImages("png");
}

void generateLanguages() {
  namespace fs = std::filesystem;
  const std::string scriptFolder = "scripts/";
  const std::string outputFolder = "languages/";
  const std::string imageFolder = "images/";

  if (!fs::exists(outputFolder)) {
    fs::create_directory(outputFolder);
  }

  auto totalStart = std::chrono::high_resolution_clock::now();
  std::mutex printMutex;
  std::vector<std::future<void>> futures;

  for (const auto& entry : fs::directory_iterator(scriptFolder)) {
    if (entry.is_regular_file()) {
      std::string filename = entry.path().filename().string();
      if (filename.size() >= 7 && filename.substr(filename.size() - 7) == ".script" &&
          filename.find('.') == filename.rfind('.')) {
        // Only process "basename.script", not "basename.script1", "basename.scriptExtra", etc.

        // Launch async task
        futures.emplace_back(std::async(std::launch::async, [=, &printMutex]() {
          std::string fullPath = scriptFolder + filename;
          std::string baseName = filename.substr(0, filename.size() - 7);
          std::string outputFilename = outputFolder + baseName + ".json";

          if (fs::exists(outputFilename)) {
            std::lock_guard<std::mutex> lock(printMutex);
            std::cout << "Skipping (already exists): " << outputFilename << std::endl;
            return;
          }

          auto start = std::chrono::high_resolution_clock::now();

          Norkella::DFA dfa;
          try {
            dfa = fileToDFA(fullPath);

            // Try to find and merge .script1, .script2, etc.
            int suffix = 1;
            while (true) {
              std::string extraFilename = scriptFolder + baseName + ".script" + std::to_string(suffix);
              if (!fs::exists(extraFilename)) break;
              Norkella::DFA dfaExtra = fileToDFA(extraFilename);
              std::set<char> fullAlphabet = dfa.getAlphabet();
              fullAlphabet.insert(dfaExtra.getAlphabet().begin(), dfaExtra.getAlphabet().end());
              dfa.expandAlphabet(fullAlphabet);
              dfa = Norkella::DFA(dfa, dfaExtra, false);
              ++suffix;
            }
            dfa = dfa.minimize();
          } catch (const std::exception& e) {
            std::lock_guard<std::mutex> lock(printMutex);
            std::cerr << "Error generating DFA from '" << filename << "': " << e.what() << std::endl;
            return;
          }

          try {
            std::string svgFilename = imageFolder + baseName + ".svg";
            if (fs::exists(svgFilename)) {
              fs::remove(svgFilename);
              std::lock_guard<std::mutex> lock(printMutex);
              std::cout << "Removed: " << svgFilename << std::endl;
            }
            std::string pngFilename = imageFolder + baseName + ".png";
            if (fs::exists(pngFilename)) {
              fs::remove(pngFilename);
              std::lock_guard<std::mutex> lock(printMutex);
              std::cout << "Removed: " << pngFilename << std::endl;
            }

            dfa.saveToFile(outputFilename);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            std::lock_guard<std::mutex> lock(printMutex);
            std::cout << "Processed: " << filename << " -> " << outputFilename
                      << " [" << duration << " ms]" << std::endl;
          } catch (const std::exception& e) {
            std::lock_guard<std::mutex> lock(printMutex);
            std::cerr << "Error saving DFA for " << filename << ": " << e.what() << std::endl;
          }
        }));
      }
    }
  }

  for (auto& fut : futures) {
    fut.get();
  }

  auto totalEnd = std::chrono::high_resolution_clock::now();
  auto totalDuration = std::chrono::duration_cast<std::chrono::milliseconds>(totalEnd - totalStart).count();
  std::cout << "Total processing time: " << totalDuration << " ms" << std::endl;
}


std::string escapeSpecialChars(const std::string& input) {
  std::unordered_set<char> specialChars = {'\\', '(', ')', '*', '^', '.'};
  std::string result;

  for (char ch : input) {
    if (specialChars.count(ch)) {
      result += '\\';
    }
    result += ch;
  }

  return result;
}

std::string escapeNonAscii(const std::string& input) {
  std::ostringstream escaped;
  for (size_t i = 0; i < input.size(); ) {
    unsigned char c = input[i];
    if (c < 0x80) {
      // ASCII character
      escaped << c;
      ++i;
    } else {
      // UTF-8 multibyte character
      uint32_t codepoint = 0;
      size_t extraBytes = 0;

      if ((c & 0xE0) == 0xC0) {        // 2 bytes
        codepoint = c & 0x1F;
        extraBytes = 1;
      } else if ((c & 0xF0) == 0xE0) { // 3 bytes
        codepoint = c & 0x0F;
        extraBytes = 2;
      } else if ((c & 0xF8) == 0xF0) { // 4 bytes
        codepoint = c & 0x07;
        extraBytes = 3;
      } else {
        // Invalid start byte
        ++i;
        continue;
      }

      if (i + extraBytes >= input.size()) {
        // Truncated multibyte sequence
        break;
      }

      bool valid = true;
      for (size_t j = 1; j <= extraBytes; ++j) {
        unsigned char cc = input[i + j];
        if ((cc & 0xC0) != 0x80) {
          codepoint = 0xFFFD; // Replacement character
          valid = false;
          break;
        }
        codepoint = (codepoint << 6) | (cc & 0x3F);
      }

      escaped << "&" << encodeBase64(codepoint) << ";";
      i += extraBytes + 1;
    }
  }
  return escaped.str();
}

Adriaan::RE fileToRegex(const std::string &filename) {
  std::string string;
  std::ifstream file(filename);
  std::string line;
  bool foundRegexSection = false;
  bool foundWordList = false;

  bool noSpacesForCaretStar = (
    filename == "scripts/C++.script" ||
    filename == "scripts/HTML.script" ||
    filename == "scripts/Python.script"
  );

  if (!file.is_open()) {
    throw std::logic_error("Error: Could not open file '" + filename + "'");
  }

  while (std::getline(file, line)) {
    if (!foundWordList && !foundRegexSection) {
      if (line == "#regex") {
        foundRegexSection = true;
      }
      if (line == "#wordlist") {
        foundWordList = true;
      }
    }
    if (line.empty() || line[0] == '#') {
      continue; // Ignore comment or empty lines
    }
    line = escapeNonAscii(line);
    if (foundRegexSection) {
      string = string.empty() ? line : string + "+" + line;
    } else if (foundWordList) {
      line = escapeSpecialChars(line);
      if (noSpacesForCaretStar) {
        string = string.empty() ? "^*((" + line + ")" : string + "+(" + line + ")";
      } else {
        string = string.empty() ? "^* ((" + line + ")" : string + "+(" + line + ")";
      }
    }
  }
  if (foundWordList) {
    if (noSpacesForCaretStar) {
      string += ")^*";
    } else {
      string += ") ^*";
    }
  }

  return {string, '\?'};
}

Norkella::DFA regexToDFA(const Adriaan::RE &in) {
  std::cout << "Original expression: \"" << in.getExpression() << "\"\n";
  std::cout << "RE\n";
  auto aenfa = in.toENFA();
  std::cout << " -> eNFA";
  Norkella::ENFA enfa(aenfa);
  std::cout << " -> Converted eNFA";
  Norkella::DFA dfa = enfa.toDFA(true);
  std::cout << " -> DFA\n";
  dfa = dfa.minimize();
  std::cout << " -> Minimized DFA" << std::endl;
  return dfa;
}

Norkella::DFA fileToDFA(const std::string &filename) {
  return regexToDFA(fileToRegex(filename));
}
} //Languages