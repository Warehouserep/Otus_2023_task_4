#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <tuple>
#include <type_traits>

template<typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
std::string type_detector(const T &ip){
	std::stringstream out;
	for (char i = sizeof(ip)-1; i >= 0; --i) {
		out<<(ip>>(i*8)&0xff);
		if (i>0) {
			out<<".";
		}
	}
	return out.str();
}

std::string type_detector(std::string ip){
	return ip;
}

template<typename Container, typename = decltype(std::declval<Container>().begin()), typename = decltype(std::declval<Container>().cend())>
std::string type_detector(const Container &ip){
	std::stringstream out;
	for (auto it = ip.begin(); it!=ip.end(); ++it) {
		if (it!=ip.begin()) {
			out<<".";
		}
		out<<*it;
	}
	return out.str();
}

template<size_t ind, typename... T>
std::string type_detector(std::tuple<T...> ip) {
	std::stringstream out;
	if (ind > 0) {
		out << ".";
	}
	out << std::get<ind>(ip);
	if constexpr(ind < sizeof...(T) - 1) {
		static_assert(std::is_same_v<decltype(std::get<ind>(ip)), decltype(std::get<ind+1>(ip))>, "different types in tuple!");
		out << type_detector<ind+1, T...>(ip);
	}
	return out.str();
}

template<typename... T>
std::string type_detector(std::tuple<T...> ip) {
	return type_detector<0, T...>(ip);
}

template<typename T>
void print_ip(T ip) {
	std::cout << type_detector(ip) << std::endl;
}

 int main()
{
	print_ip(char(-1));
	print_ip(short(0));
	print_ip(int(2130706433));
	print_ip((long long)(8875824491850138409));
	print_ip("Hello, World!");
	print_ip(std::vector<int>{100, 200, 300, 400}); // 100.200.300.400
	print_ip(std::list<short>{400, 300, 200, 100}); // 400.300.200.100
	print_ip(std::make_tuple(123, 456, 789, 0)); // 123.456.789.0

	return 0;
}

