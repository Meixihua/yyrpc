#include <tuple>
#include <string>
#include <functional>
#include <type_traits>

namespace rpc
{
	struct Response
	{
		int r;
		std::string s;
	};

	struct Request
	{
		double f;
		bool b;
	};

	//����RPC����
	Response GetResponseTest1(void);
	Response GetResponseTest2(int x, int y);
	Response GetResponseTest3(std::string s);
	Response GetResponseTest4(std::string &s, int x);
	Response GetResponseTest5(const Request res, int x);
}

template<typename T>
struct function_traits;

template<typename R, typename ...Args>
struct function_traits< R(Args...) > {
	static constexpr std::size_t nargs = sizeof...(Args);
	using return_type = R;
	template <std::size_t index>
	struct argument
	{
		static_assert(index < nargs, "RPC_CALL_ERROR: invalid parameter index.");
		using type = typename std::tuple_element<index, std::tuple<Args...>>::type;
	};
};

template<typename R, typename ...Args>
struct function_traits< R(*)(Args...) > : public function_traits<R(Args...)> {
};

template<typename R, typename base, typename ...Args>
struct function_traits< R(base::*)(Args...) > : public function_traits<R(Args...)> {
	using base_type = base;
};

template<typename R>
struct function_traits< R(void) > {
	static constexpr std::size_t nargs = 0;
	using return_type = R;
};

template<typename R>
struct function_traits< R(*)(void) > : public function_traits<R(void)> {
};

template<typename R, typename base >
struct function_traits< R(base::*)(void) > : public function_traits<R(void)> {
};

template<typename R, typename ...Args>
struct function_traits< std::function<R(Args...)> > {
	static const size_t nargs = sizeof...(Args);
	using return_type = R;
	template <size_t index>
	struct argument {
		static_assert(index < nargs, "RPC_CALL_ERROR: invalid parameter index.");
		using type = typename std::tuple_element<index, std::tuple<Args...>>::type;
	};
};

template<typename Func, typename... Args>
void RpcFunctionTraitsCheck(Func func, Args ...args) {
	using traits = function_traits<decltype(func)>;
	static_assert(traits::nargs != 0, "RPC_CALL_ERROR: pass arguments to a void arguments signature function.");
	static_assert(sizeof...(args) == traits::nargs, "RPC_CALL_ERROR: the number of argument is not equal.");

	TupleUnpacker<sizeof...(args)-1, decltype(func)>::unpackCheckHelper(func, args...);
}

template<typename Func>
void RpcFunctionTraitsCheck(Func func) {
	using traits = function_traits<decltype(func)>;
	static_assert(traits::nargs == 0, "RPC_CALL_ERROR: the number of argument is not equal.");
}

template< std::size_t I, typename Func>
struct TupleUnpacker {
	template <typename... Args>
	static void unpackCheckHelper(Func func, Args ...args) {
		auto myTuple = std::make_tuple(args...);
		using traits = function_traits<decltype(func)>;
		static_assert(std::is_same<traits::argument<I>::type, std::tuple_element<I, decltype(myTuple) >::type>::value,
			"RPC_CALL_ERROR: some invalid argument type");
		TupleUnpacker<I - 1, decltype(func)>::unpackCheckHelper(func, args...);
	}
};

template<typename Func>
struct TupleUnpacker <0, Func> {
	template <typename... Args>
	static void unpackCheckHelper(Func func, Args ...args) {
		auto myTuple = std::make_tuple(args...);
		using traits = function_traits<decltype(func)>;
		static_assert(std::is_same<traits::argument<0>::type, std::tuple_element<0, decltype(myTuple) >::type>::value,
			"RPC_CALL_ERROR: some invalid argument type");
	}
};

template<typename Func, typename... Args>
auto SyncCall(const char* method_name, Func func, Args ...args) {
	RpcFunctionTraitsCheck(func, args...);
}

template<typename Func>
auto SyncCall(const char* method_name, Func func) {
	RpcFunctionTraitsCheck(func);
}

#define SYNC_CALL(methed_name, ...) \
  SyncCall(#methed_name, methed_name, ##__VA_ARGS__)

int main() {
	int x, y;
	std::string str, *pstr;
	rpc::Request req, *preq;
	const rpc::Request creq;
	/*
		Response GetResponseTest1(void);
		Response GetResponseTest2(int x, int y);
		Response GetResponseTest3(std::string s);
		Response GetResponseTest4(std::string &s, int x);
		Response GetResponseTest5(const Request res, int x);
		Response GetResponseTest5(const Request res, int x, int y, int z, int m, int n);
	*/
	SYNC_CALL(rpc::GetResponseTest1);				//accept
	SYNC_CALL(rpc::GetResponseTest1, x);			//compiler error:��һ����ǩ���ķ�����ֵ

	SYNC_CALL(rpc::GetResponseTest2, x, y);			//accepet
	SYNC_CALL(rpc::GetResponseTest2);				//compiler error:����������ƥ��
	SYNC_CALL(rpc::GetResponseTest2, x);			//compiler error:����������ƥ��
	SYNC_CALL(rpc::GetResponseTest2, str, y);		//compiler error:�������Ͳ�ƥ��

	SYNC_CALL(rpc::GetResponseTest3, str);			//accept
	SYNC_CALL(rpc::GetResponseTest3, pstr);			//compiler error:�������Ͳ�ƥ��

	SYNC_CALL(rpc::GetResponseTest4, str, x);		//accept

	SYNC_CALL(rpc::GetResponseTest5, req, x);		//accept
	SYNC_CALL(rpc::GetResponseTest5, creq, x);		//accept
	SYNC_CALL(rpc::GetResponseTest5, preq, x);		//compiler error:�������Ͳ�ƥ��
}
