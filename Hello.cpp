#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <hash_set>
#include <set>
#include <string>

namespace __gnu_cxx {
    template<>
    struct hash<std::string>
    {
        hash<char*> h;
        size_t operator()(const std::string &s) const
        {
            return h(s.c_str());
        };
    };
}

using namespace std;
using namespace __gnu_cxx;
//using namespace stdext;//hash_set

int main(){
	printf("Hello World!\n");
	int i = 0;
	hash_set<int> hset;
	for(int i=0;i<10;i++)
		hset.insert(i);
	hset.insert(1);
	for(auto x:hset)
		cout<< x<<endl;	
	hash_set<string> set1;
	set1.insert("hello");
	set1.insert("world");
	for(auto s:set1)
		cout<< s<<endl;
}