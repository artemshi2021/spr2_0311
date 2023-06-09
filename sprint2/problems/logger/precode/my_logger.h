#pragma once

#include <chrono>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <optional>
#include <mutex>
#include <thread>

using namespace std::literals;

#define LOG(...) Logger::GetInstance().Log(__VA_ARGS__)

class Logger {
    auto GetTime() const {
        if (manual_ts_) {
            return *manual_ts_;
        }

        return std::chrono::system_clock::now();
    }

    auto GetTimeStamp() const {
        const auto now = GetTime();
        const auto t_c = std::chrono::system_clock::to_time_t(now);
        return std::put_time(std::localtime(&t_c), "%F %T");
    }

    // Для имени файла возьмите дату с форматом "%Y_%m_%d"
    std::string GetFileTimeStamp() const;

    Logger() 
    {
      Init();
    }
    Logger(const Logger&) = delete;

public:
    static Logger& GetInstance() {
        static Logger obj;
        return obj;
    }

    // Выведите в поток все аргументы.
    template<class... Ts>
    void Log(const Ts&... args);

    // Установите manual_ts_. Учтите, что эта операция может выполняться
    // параллельно с выводом в поток, вам нужно предусмотреть 
    // синхронизацию.
    void SetTimestamp(std::chrono::system_clock::time_point ts);

private:
    std::optional<std::chrono::system_clock::time_point> manual_ts_;
    std::optional<std::ofstream> log_file_ ;//{GetFileTimeStamp() , std::ios::app  };
public:
    void Init()
    {
       auto now = std::chrono::system_clock::now() /*-std::chrono::days(100) */;    
       SetTimestamp(now);
       log_file_ = InitFile() ;
    }
    std::optional<std::ofstream> InitFile()
    {
       std::ofstream lt {GetFileTimeStamp() , std::ios::app  } ;
       
       return lt ;
    }    
    
};
