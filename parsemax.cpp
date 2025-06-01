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


std::string MaxChargingPower(std::string_view const & previousMetadata, std::string const & k, std::string v)
{
  std::string orig = v;

  static constexpr char kMaxPowerDC[]=" kW DC";
  static constexpr char kMaxPowerAC[]=" kW AC";
  static constexpr char kMaxPowerDelim[]=", ";
  static constexpr char kMaxPowerPoint='.';
  size_t pos = 0;
  std::string dc, ac, prev = static_cast<std::string>(previousMetadata);

  if (prev.size() > 1) {
    if ((pos = prev.find(kMaxPowerDelim)) != std::string::npos) {
      dc = prev.substr(0, pos - (sizeof(kMaxPowerDC) - 1));
      ac = prev.substr(pos + sizeof(kMaxPowerDelim) - 1, prev.size() - (pos + sizeof(kMaxPowerDelim) + sizeof(kMaxPowerAC) - 2));

      if (kMaxPowerPoint != '.') {
        std::replace(dc.begin(), dc.end(), kMaxPowerPoint, '.');
        std::replace(ac.begin(), ac.end(), kMaxPowerPoint, '.');
      }
    } else if ((pos = prev.find(kMaxPowerDC)) != std::string::npos) {
      dc = prev.substr(0, pos);
      if (kMaxPowerPoint != '.')
        std::replace(dc.begin(), dc.end(), kMaxPowerPoint, '.');
    } else if ((pos = prev.find(kMaxPowerAC)) != std::string::npos) {
      ac = prev.substr(0, pos);
      if (kMaxPowerPoint != '.')
        std::replace(ac.begin(), ac.end(), kMaxPowerPoint, '.');
    }
  }

  while ((pos = v.find("к")) != std::string::npos) {
    v[pos] = 'k';
    v[pos + 1] = ' ';
  }

  while ((pos = v.find("К")) != std::string::npos) {
    v[pos] = 'k';
    v[pos + 1] = ' ';
  }

  while ((pos = v.find("в")) != std::string::npos) {
    v[pos] = 'w';
    v[pos + 1] = ' ';
  }

  while ((pos = v.find("В")) != std::string::npos) {
    v[pos] = 'w';
    v[pos + 1] = ' ';
  }

  std::transform(v.begin(), v.end(), v.begin(), ::tolower);

  if (v.find("http") != std::string::npos) {
    std::cout<<std::left<<std::setw(40)<<("\""+k+"\"")<<std::left<<std::setw(64)<<("\""+orig+"\"")<<std::left<<std::setw(64)<<("\""+v+"\"")<<std::left<<std::setw(8)<<("\"\"")<<"\"NA\"   "<<("\""+prev+"\"")<<std::endl;
    return prev;
  }

  v.erase(std::remove_if(v.begin(), v.end(), ::isspace), v.end());

  char split = ' ';
  size_t mult = 0;

  if ((pos = v.find(';')) != std::string::npos)
    split = ';';
  else if ((pos = v.find('+')) != std::string::npos)
    split = '+';
  else if ((pos = v.find('-')) != std::string::npos)
    split = '-';
  else if ((pos = v.find('=')) != std::string::npos)
    split = '=';
  else if ((pos = v.find(':')) != std::string::npos)
    split = ':';
  else if ((pos = v.find('/')) != std::string::npos)
    split = '/';
  else if ((pos = v.find('\\')) != std::string::npos)
    split = '\\';
  else if ((pos = v.find(',')) != std::string::npos) {
    if (((mult = v.find("kw")) != std::string::npos) && (v.find("kw", mult + 1) != std::string::npos)) {
      split = ',';
    } else {
      split = ' ';
      pos = v.size();
    }
  } else
    pos = v.size();

  bool typedc = false;

  if ((k.find("combo") != std::string::npos) || (k.find("chademo") != std::string::npos) ||
      (k.find("super") != std::string::npos) || (k.find("bosch") != std::string::npos) ||
      (k.find("gb_dc") != std::string::npos) || (k.find("3105") != std::string::npos) ||
      (k.find("roadster") != std::string::npos))
    typedc = true;

  size_t last = 0, watts = 0;
  float numeric = 0.0, maximum = 0.0;
  std::string part, result;

  do {
    part = v.substr(last, pos - last);
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
    part.erase(std::remove_if(part.begin(), part.end(), [](unsigned char c) { return ((!std::isdigit(c)) && (c != '.')); }), part.end());
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
    }

    if (numeric > maximum) {
      if (((mult = part.find('.')) != std::string::npos) && (mult < (part.size() - 2))) {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(numeric < 100.0 ? 1 : 0) << numeric;
        part = stream.str();
      }

      maximum = numeric;

      if ((part.find('.') != std::string::npos) && (part[part.size() - 1] == '0'))
        result = part.substr(0, part.size() - 2);
      else
        result = part;
    }

    last = pos;
    pos = v.find(split, last + 1);
  } while ((last++) < (v.size() - 1));

  if (maximum == 0.0) {
    std::cout<<std::left<<std::setw(40)<<("\""+k+"\"")<<std::left<<std::setw(64)<<("\""+orig+"\"")<<std::left<<std::setw(64)<<("\""+v+"\"")<<std::left<<std::setw(8)<<("\""+result+"\"")<<(typedc?"\"DC\"   ":"\"AC\"   ")<<("\""+prev+"\"")<<std::endl;
    return prev;
  }

  if (((k.find("nacs") != std::string::npos) || (k == "charging_station:output")) && (maximum >= 50.0))
    typedc = true;
  if ((typedc) && (((dc.empty()) || (maximum > std::stof(dc)))))
    dc = result;
  if ((!typedc) && (((ac.empty()) || (maximum > std::stof(ac)))))
    ac = result;

  if (!dc.empty()) {
    if (!ac.empty()) {
      if (kMaxPowerPoint != '.')
        std::replace(dc.begin(), dc.end(), '.', kMaxPowerPoint);
      if (kMaxPowerPoint != '.')
        std::replace(ac.begin(), ac.end(), '.', kMaxPowerPoint);
      prev = dc + kMaxPowerDC + kMaxPowerDelim + ac + kMaxPowerAC;
    } else {
      if (kMaxPowerPoint != '.')
        std::replace(dc.begin(), dc.end(), '.', kMaxPowerPoint);
      prev = dc + kMaxPowerDC;
    }
  } else if (!ac.empty()) {
    if (kMaxPowerPoint != '.')
      std::replace(ac.begin(), ac.end(), '.', kMaxPowerPoint);
    prev = ac + kMaxPowerAC;
  }

  std::cout<<std::left<<std::setw(40)<<("\""+k+"\"")<<std::left<<std::setw(64)<<("\""+orig+"\"")<<std::left<<std::setw(64)<<("\""+v+"\"")<<std::left<<std::setw(8)<<("\""+result+"\"")<<(typedc?"\"DC\"   ":"\"AC\"   ")<<("\""+prev+"\"")<<std::endl;
  return prev;
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
