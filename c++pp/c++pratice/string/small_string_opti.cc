#include<iostream>
#include<string>

using namespace std;

void* operator new(size_t count)
{
    cout << "  " << count << "bytes" << endl;
    return malloc(count);
}

void GetString(const string&) { }
void GetStringView(string_view strView) { }

int main()
{
    cout << endl;
    cout << "std::string" << endl;
    string large = "0123456789-123456789-123456789-123456789";
    string substr = small.substr(10);
    cout << "  " << substr << endl;
    cout << endl;

    cout << "string_view" << endl;
    // GetString(small);
    // GetString("0123456789");

    string_view largeStringView{large.c_str(), large.size()};
    const char message[] = "0123456789";
    GetString(message);

    cout << endl;

    return 0;
}