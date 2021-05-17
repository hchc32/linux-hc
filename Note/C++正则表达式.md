#### c++正则表达式

##### 1.regex_match()

> 返回值:目标序列与是否与正则表达式rgx匹配

```c++
#include<regex>
using namespace std;
string s("subject");
regex  e("(sub)(.*)");
regex_match(s,e);
```

