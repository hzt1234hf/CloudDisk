# CloudDisk
基于Flask和React实现的网页端云网盘

## 本工程基于实验楼课程修改
- 课程地址：[https://www.shiyanlou.com/courses/1212](https://www.shiyanlou.com/courses/1212)
- 项目技术：
    1. 使用了React前端框架和Flask基于Python的后台开发框架
    2. ORM层使用了peewee与sqlite数据库通信
    3. 使用token机制和cookie实现验证
- 项目功能：
    1. 创建目录，文件上传、下载功能，目录不可嵌套，且本地无法直接访问源文件，本地文件操作无法反应到数据库和前端界面
    2. 文件公开分享、私密分享功能
- 注意：基本代码与教程一致，修改的内容如下：
    - 使用了Bootstrap4代替了Bootstrap3，图标库使用了Awesome和Octicon，使用方式：
        - [react-fontawesome](https://github.com/FortAwesome/react-fontawesome)   
        - [react-octicon](https://github.com/insin/react-octicon)
    - 修改教程中部分错误代码，请与源码对比
- 使用：请按照所需的脚本和库下载相应的文件，服务器端带有已经部署好的前端代码，可以在服务器所需插件下载完后直接运行查看效果为

