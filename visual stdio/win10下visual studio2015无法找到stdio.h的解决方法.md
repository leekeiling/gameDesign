## win10下visual studio2015无法找到stdio.h的解决方法

据说win10和vs2015最配哦，然而开开心心打了个helloworld后发现~

fatal error C1083: 无法打开包括文件: "stdlio.h": No such file or directory

看了很多参考，都是类似于添加库文件的解决方法（可以参考http://www.zhimengzhe.com/windows/30406.html），然而搜了一下电脑。也不知是不是安装了n遍的原因，居然没有stdio.h!!!
所以问题就很简单了，其实看了一下vs是链接了C:\Program Files (x86)\Windows Kits\10\Lib\10.0.10240.0\ucrt这个路径的，所以下载相关.h文件到这里后，就顺利运行了