#include <chrono>
#include <string_view>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <thread>
#include <syncstream>
#include "my_logger.h"

using namespace std;
namespace conv
{
  template <class T>
  string to_str(const T &val)
  {
    using std::to_string;
    return to_string(val);
  }

  inline string to_str(const char *p)
  {
    return p;
  }
  
  inline const string& to_str(const string &s)
  {
    return s;
  }
}

template<class... Ts>
    void Logger::Log(const Ts&... args)
{
     std::osyncstream synced_out(*log_file_) ;
     *log_file_ << GetTimeStamp() << ": " ;
     
     using std::to_string;
     string param[] { conv::to_str(args)... };

     bool fst = true ;
     for (  auto& s : param )
     {
         *log_file_ << ( !fst ? "," : "" ) << s  ;
         fst = false ;
     }
     *log_file_ << std::endl;
}

void Logger::SetTimestamp(std::chrono::system_clock::time_point ts)
{
   bool mdf = false ;
   if ( manual_ts_ != std::nullopt )
      mdf = true ;

   manual_ts_ = ts ;
   if ( mdf )
   {
      log_file_ = InitFile() ;
     // std::cout << "New file " << GetFileTimeStamp() << std::endl;
   }
}

std::string Logger::GetFileTimeStamp() const
{
   std::stringstream ss;
   
   const auto now = std::chrono::system_clock::now();
   const auto t_c = std::chrono::system_clock::to_time_t( *manual_ts_ );
   
//       /*"/var/log/"sample_log_"*/"sample_log_"+
   ss << std::put_time(std::localtime(&t_c), "/var/log/sample_log_%Y-%m-%d.log") ;
   return ss.str();
}

int main() {

  LOG ("Date 1") ;
  //change date
  auto now = std::chrono::system_clock::now() - std::chrono::days(100);
  Logger::GetInstance().SetTimestamp(now);
  LOG ("Date 2") ;


  now = std::chrono::system_clock::now() + std::chrono::years(1);
  Logger::GetInstance().SetTimestamp(now);
  LOG ("Date 3") ;
  
  return 0;
}
