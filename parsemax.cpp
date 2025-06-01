//
// Arthur Matteson, 2025/05/31
// Compile with `g++ -o parsemax parsemax.cpp`
//


#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <bits/stdc++.h>


#define MAX_POWER_RESET 3
#define MAX_POWER_DCSTR " kW DC"
#define MAX_POWER_ACSTR " kW AC"
#define MAX_POWER_DELIM ", "
#define MAX_POWER_POINT '.'


std::string MaxChargingPower(std::string_view const & m, std::string const & k, std::string const & v)
{
  bool typedc = false;
  char split = ' ';
  size_t pos = 0, last = 0, mult = 0, watts = 0;
  float numeric = 0.0, maximum = 0.0;
  std::string dc, ac, lower = v, part, result;

  std::string value = static_cast<std::string>(m);
  if (v.find("http") != std::string::npos)
    goto skip;
  if (MAX_POWER_POINT != '.')
    std::replace(dc.begin(), dc.end(), MAX_POWER_POINT, '.');

  if (value.size() > 5) {
    if ((pos = value.find(MAX_POWER_DELIM)) != std::string::npos) {
      dc = value.substr(0, value.find_first_not_of("0123456789.,"));
      ac = value.substr(pos + 2, value.find_first_not_of("0123456789.,", pos + 3) - (pos + 2));
    } else if (value.at(value.size() - 2) == 'D') {
      dc = value.substr(0, value.find_first_not_of("0123456789.,"));
    } else if (value.at(value.size() - 2) == 'A') {
      ac = value.substr(0, value.find_first_not_of("0123456789.,"));
    }
  }

  while ((pos = lower.find("к")) != std::string::npos) {
    lower[pos] = 'k';
    lower[pos + 1] = ' ';
  }

  while ((pos = lower.find("К")) != std::string::npos) {
    lower[pos] = 'k';
    lower[pos + 1] = ' ';
  }

  while ((pos = lower.find("в")) != std::string::npos) {
    lower[pos] = 'w';
    lower[pos + 1] = ' ';
  }

  while ((pos = lower.find("В")) != std::string::npos) {
    lower[pos] = 'w';
    lower[pos + 1] = ' ';
  }

  std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
  lower.erase(std::remove_if(lower.begin(), lower.end(), ::isspace), lower.end());

  if ((pos = lower.find(';')) != std::string::npos) {
    split = ';';
  } else if ((pos = lower.find('+')) != std::string::npos) {
    split = '+';
  } else if ((pos = lower.find('-')) != std::string::npos) {
    split = '-';
  } else if ((pos = lower.find('=')) != std::string::npos) {
    split = '=';
  } else if ((pos = lower.find(':')) != std::string::npos) {
    split = ':';
  } else if ((pos = lower.find('/')) != std::string::npos) {
    split = '/';
  } else if ((pos = lower.find('\\')) != std::string::npos) {
    split = '\\';
  } else if ((pos = lower.find(',')) != std::string::npos) {
    if (((mult = lower.find("kw")) != std::string::npos) && (lower.find("kw", mult + 1) != std::string::npos)) {
      split = ',';
    } else {
      split = ' ';
      pos = lower.size();
    }
  } else {
    pos = lower.size();
  }

  if ((k.find("combo") != std::string::npos) || (k.find("chademo") != std::string::npos) ||
      (k.find("super") != std::string::npos) || (k.find("bosch") != std::string::npos) ||
      (k.find("gb_dc") != std::string::npos) || (k.find("3105") != std::string::npos) ||
      (k.find("roadster") != std::string::npos))
    typedc = true;

  do {
    part = lower.substr(last, pos - last);
    std::replace(part.begin(), part.end(), ',', '.');
    if (((mult = part.find('(')) != std::string::npos) && ((watts = part.find(')', mult + 1)) != std::string::npos))
      part.erase(mult, mult - watts);

    if ((mult = part.find('x')) != std::string::npos) {
      if (mult < ((part.size() + 1) >> 1)) {
        part = part.substr(mult + 1, part.size() - (mult + 1));
      } else {
        part = part.substr(0, mult);
      }
    } else if ((mult = part.find('*')) != std::string::npos) {
      if (mult < ((part.size() + 1) >> 1)) {
        part = part.substr(mult + 1, part.size() - (mult + 1));
      } else {
        part = part.substr(0, mult);
      }
    } else if ((mult = part.find("×")) != std::string::npos) {
      if (mult < (part.size() >> 1)) {
        part = part.substr(mult + 2, part.size() - (mult + 2));
      } else {
        part = part.substr(0, mult);
      }
    }

    if ((watts = part.find('w')) != std::string::npos)
      watts = ((part.find('k') == std::string::npos) ? 0 : std::string::npos);
    if ((mult = part.find("kva")) != std::string::npos)
      part = part.substr(0, mult);
    if ((mult = part.find('w')) != std::string::npos)
      part = part.substr(0, mult);
    mult = part.size();
    part.erase(
      std::remove_if(part.begin(), part.end(), [](unsigned char c) { return ((!std::isdigit(c)) && (c != '.')); }),
      part.end());
    if (part.size() < (mult >> 1))
      part = "";

    try {
      numeric = std::stof(part);
    } catch (const std::invalid_argument& e) {
      numeric = 0.0;
    } catch (const std::out_of_range& e) {
      numeric = 0.0;
    }

    if ((watts != std::string::npos) || (numeric > 2000.0)) {
      numeric /= 1000.0;
      std::stringstream stream;
      stream << std::fixed << std::setprecision(numeric < 100.0 ? 1 : 0) << numeric;
      part = stream.str();
    } else if ((numeric > 0.0) && ((mult = part.find('.')) != std::string::npos) && (mult < (part.size() - 2))) {
      std::stringstream stream;
      stream << std::fixed << std::setprecision(numeric < 100.0 ? 1 : 0) << numeric;
      part = stream.str();
    }

    if (numeric > maximum) {
      maximum = numeric;
      result = part;
    }

    last = pos;
    pos = lower.find(split, last + 1);
  } while ((last++) < (lower.size() - 1));

  if (maximum == 0.0)
    goto skip;

  if (((k.find("nacs") != std::string::npos) || (k == "charging_station:output")) && (maximum >= 50.0))
    typedc = true;
  if ((typedc) && (((dc.empty()) || (maximum > std::stof(dc)))))
    dc = result;
  if ((!typedc) && (((ac.empty()) || (maximum > std::stof(ac)))))
    ac = result;

  if (!dc.empty()) {
    if (!ac.empty()) {
      if (MAX_POWER_POINT != '.')
        std::replace(dc.begin(), dc.end(), '.', MAX_POWER_POINT);
      if (MAX_POWER_POINT != '.')
        std::replace(ac.begin(), ac.end(), '.', MAX_POWER_POINT);
      value = dc + MAX_POWER_DCSTR + MAX_POWER_DELIM + ac + MAX_POWER_ACSTR;
    } else {
      if (MAX_POWER_POINT != '.')
        std::replace(dc.begin(), dc.end(), '.', MAX_POWER_POINT);
      value = dc + MAX_POWER_DCSTR;
    }
  } else if (!ac.empty()) {
    if (MAX_POWER_POINT != '.')
      std::replace(ac.begin(), ac.end(), '.', MAX_POWER_POINT);
    value = ac + MAX_POWER_ACSTR;
  }

skip:
  std::cout<<std::left<<std::setw(40)<<("\""+k+"\"")<<std::left<<std::setw(64)<<("\""+v+"\"")<<std::left<<std::setw(64)<<("\""+lower+"\"")<<std::left<<std::setw(8)<<("\""+result+"\"")<<(typedc?"\"DC\"   ":"\"AC\"   ")<<("\""+value+"\"")<<std::endl;

  return value;
}


int main(int argc, char *argv[])
{
  int reset=0, kf=0, vf=0;
  std::string line, present;

  if(argc!=2) return 1;
  std::ifstream fin(argv[1]);
  if(!fin.is_open()) return 1;

  while(std::getline(fin, line)) {
    kf=line.find("k="); vf=line.find("v=");
    present=MaxChargingPower(static_cast<std::string_view>(present), line.substr(kf+3, vf-(kf+5)), line.substr(vf+3, line.size()-(vf+6)));

    if((++reset)>=MAX_POWER_RESET) {
      reset=0; present="";
    }
  }

  fin.close();
  return 0;
}
