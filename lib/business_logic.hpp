#pragma once

#include <cpr/cpr.h>

#include <string>
#include <unordered_map>
#include <vector>

/*
найти маршруты из СПб в город и обратно
с максимум 1 пересадкой
на заданную дату
*/


struct RouteRequest {
  RouteRequest(std::vector<std::string>);

  std::string from_;
  std::string to_;
  std::string date_;
  std::string transport_type_;
  std::string limit_;
};

// struct Route {
//   // std::string from_;
//   // std::string to_;
//   // std::string date_;
// };

struct IRouteRepository {
  virtual void GetRoute() = 0;
  virtual ~IRouteRepository();
};

struct IYandexRepository : IRouteRepository {
 
  void GetRoute();

  void VisitAPI();

  private:
  int a;
};

struct CacheRepository {
  void GetRoute();
 public:
  
 private:
  IYandexRepository api_repo_;
};

struct RouteRespone {};

struct DataRequester {
  DataRequester(const RouteRequest& request);

  void ProcessRequest() {
    
  }

 private:
  const RouteRequest& request_;
  IRouteRepository* repo_;
};

struct DataProcesser {};

struct DataPrinter {};
