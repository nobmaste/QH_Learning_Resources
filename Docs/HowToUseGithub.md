## 如何安装git
什么是git？git是一种版本控制技术，github使用了git技术但git技术不是github专属，更多细节不多赘述。<br>
 git下载及安装
 1. 可以去官网https://git-scm.com/下载安装包
 2. 安装步骤可以直接默认就行，有需要就修改安装路径。其次在选择安装组件时记得windowsExplorerIntegration下的两个选项、GitLFS选项以及下面两个Associate选项都勾上，其它一路默认最后安装，安装完打开GitBash，输入git --version，如果成功输出版本信息就是安装成功了。
 3. 更详细的教程可以参考博客或自行搜索：https://blog.csdn.net/weixin_42242910/article/details/136297201
## git配置ssh
 配置ssh是为了我们方便拉取和上传仓库的前置步骤
 一般配置过程可以参考如下博客：https://blog.csdn.net/weixin_42310154/article/details/118340458<br>
 说明：在配置ssh时可能会遇到配置失败，可能是端口问题也可能是需要修改根目录文件，不过离作者本人遇到报错已经很久了，不记得具体错误了，如遇到问题请自行百度或在issue留言或者直接联系本人
## 如何将仓库克隆到本地,git基本命令
 配置完前面的内容可以直接使用git命令来拉取远程仓库上的内容，当然也可以直接下载整个文件夹或者单独文件，这里我们说怎么用git命令。<br>
 首先在本地，也就是你的电脑上选一个文件夹，这个文件夹就是保存从远程仓库上下载的文件。右键git Bash Here，输入命令：git clone "ssh地址"，ssh在每个仓库里面点击code按钮就能找到，直接复制就行。然后等待下载完成就行，这里可能需要加速器来加速下载。<br>
 除了下载外还有上传到远程仓库的操作，当我们在本地上修改完代码就可以直接使用git命令将代码上传到远程仓库，详细可以参考以下博文：https://blog.csdn.net/weixin_51033461/article/details/119997189?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522171370943216800227429006%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=171370943216800227429006&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduend~default-2-119997189-null-null.142^v100^pc_search_result_base9&utm_term=git%E5%91%BD%E4%BB%A4%E4%B8%8A%E4%BC%A0%E6%96%87%E4%BB%B6%E5%88%B0%E4%BB%93%E5%BA%93&spm=1018.2226.3001.4187<br>
 使用命令的方式比较麻烦，每次都要输入命令，如果你熟悉vscode的话可以参考以下博文，这会方便很多：https://blog.csdn.net/m0_51185558/article/details/126181439?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522171370829816800227475689%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=171370829816800227475689&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_positive~default-1-126181439-null-null.142^v100^pc_search_result_base9&utm_term=github%20vscode&spm=1018.2226.3001.4187
## 什么是fork、branch、issue、pr(pull request)
详细说明以后在编辑，这里先说下怎么用。<br>
star是收藏这个项目，但是如果你想参与一个项目的更新（上传）就需要先fork这个项目，**Fork 是对一个仓库的克隆**。克隆一个仓库允许你自由试验各种改变，而不影响原始的项目。<br>
在fork完一个项目后你要**在branch分支下新建一个分支**，分支一般默认是main(在code按钮同一行)，新建一个你的分支命名可是你的name_develp这样的格式，**当你将远程仓库的文件下载并修改后上传回远程仓库时要上传到你新建的分支**s。<br>
此时项目的修改只有你这改了，怎么合并到原项目中?这时就要用到pull request，在仓库中点击new pull request输入你的标题内容来描述你做了什么修改或上传了什么新内容，当审核员审核完成后会将你提交的内容合并到项目中。<br>
issue一般时项目有问题提出的地方，类似于这个项目的小论坛。
可以参考这篇博文：https://zhuanlan.zhihu.com/p/51199833
## markdown使用方法
markdown语法可以直接看菜鸟教程或参考这个文档格式