#ifndef TEST_API_H
#define TEST_API_H

#include "yyrpc.h"
#include <vector>
#include <string>

struct Request
{
  double memberDouble;
  std::string memberString;
  std::vector<int> memberVector;
  YYRPC_PROPERTY(memberDouble, memberString, memberVector);
};

struct Response
{
  std::map<int, std::string> memberMap;
  YYRPC_PROPERTY(memberMap);
};

namespace SimpleAPi
{
  YYRPC_METHOD(Hello, void());  //�޲������޷���
  YYRPC_METHOD(Sum, int(int, int)); //�򵥲���������Ӧ
}

namespace TestAPi
{
  YYRPC_METHOD(TestMethod, Response(const std::vector<int>&, const std::string&, double, const Request&));  //���Ӳ�����������Ӧ
}

#endif  //! #ifndef TEST_API_H