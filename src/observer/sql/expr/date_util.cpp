// src/observer/sql/expr/date_util.cpp
#include "date_util.h"
#include <cstdlib>
#include <cstdio>
#include <climits>

bool is_leap(int y) { return (y%4==0) && (y%100!=0 || y%400==0); }

int days_in_month(int y,int m){
  static const int dim[12]={31,28,31,30,31,30,31,31,30,31,30,31};
  if(m<1||m>12) return 0;
  if(m==2) return is_leap(y)?29:28;
  return dim[m-1];
}

bool parse_yyyy_mm_dd(const std::string &s,int &y,int &m,int &d){
  size_t l=s.find_first_not_of(' '), r=s.find_last_not_of(' ');
  if(l==std::string::npos) return false;
  std::string t=s.substr(l,r-l+1);
  if(t.size()!=10||t[4]!='-'||t[7]!='-') return false;
  auto isd=[](char c){return c>='0'&&c<='9';};
  for(int i: {0,1,2,3,5,6,8,9}) if(!isd(t[i])) return false;
  y=std::atoi(t.substr(0,4).c_str());
  m=std::atoi(t.substr(5,2).c_str());
  d=std::atoi(t.substr(8,2).c_str());
  if(y<1||y>9999) return false;
  if(m<1||m>12)   return false;
  int dim=days_in_month(y,m);
  if(d<1||d>dim)  return false;
  return true;
}

static inline int64_t days_before_year_i64(int y){
  int64_t y1=y-1;
  return 365LL*y1 + y1/4 - y1/100 + y1/400;
}
int64_t days_before_year(int y){ return days_before_year_i64(y); }

int64_t ordinal_yyyy_mm_dd(int y,int m,int d){
  static const int pref[12]={0,31,59,90,120,151,181,212,243,273,304,334};
  int64_t days = days_before_year_i64(y) + pref[m-1] + d;
  if(m>2 && is_leap(y)) days += 1;
  return days; // 0001-01-01 -> 1
}

static inline int64_t epoch_ordinal(){ return ordinal_yyyy_mm_dd(1970,1,1); }

bool date_from_string(const std::string &s,int32_t &out_days){
  int y,m,d;
  if(!parse_yyyy_mm_dd(s,y,m,d)) return false;
  int64_t diff = ordinal_yyyy_mm_dd(y,m,d) - epoch_ordinal();
  if(diff < INT32_MIN || diff > INT32_MAX) return false;
  out_days = static_cast<int32_t>(diff);
  return true;
}

void date_to_string(int32_t days,std::string &out){
  int64_t ord = epoch_ordinal() + static_cast<int64_t>(days);
  if(ord<1) ord=1;

  // 二分定位年份：days_before_year(y) < ord <= days_before_year(y+1)
  int low=1, high=9999;
  while(low<high){
    int mid=(low+high+1)/2;
    if(days_before_year_i64(mid) < ord) low=mid; else high=mid-1;
  }
  int y=low;
  int64_t doy = ord - days_before_year_i64(y);

  int m=1;
  while(true){
    int dim=days_in_month(y,m);
    if(doy>dim){ doy-=dim; m++; }
    else break;
  }
  int d = static_cast<int>(doy);

  char buf[32];
  std::snprintf(buf,sizeof(buf),"%04d-%02d-%02d",y,m,d);
  out.assign(buf);
}
