# PowerEsc Insipre From PowerEsc

PowerEsc是一个用于增强`Esc`按键功能的小工具，初衷是为了让所有应用都能像Vim一样通过`H, J, K, L`来替代`Left, Down, Up, Right`四个方向键，而`Esc`键的位置很适合用做控制键，所以在保持了`Esc`原本功能的基础上，将其改造成了控制键，实现将`Esc + H/J/K/L`映射为四个方向键的功能，以及A/E 实现Home和End。

## 使用方法

* `Esc Down`, `Esc Up`: 切换大写锁定，和`Esc`原本的功能一致；
* `Esc Down` + `H/J/K/L`: 分别转换为`Left, Down, Up, Right`四个方向键的按键事件；
* `Esc Down` + `A/E` : 转换为 Home,End

## 编译方法

* 编译环境：`mingw` (`vs c++`环境下未验证)

* 编译方法：

  ```shell
  $ make
  OR
  $ scons
  ```

* `power_Esc.exe`为编译好的二进制文件，在Windows10下测试通过；

## 安装方法

* 运行`power_Esc.exe`即可，结束应用目前只能通过任务管理器结束进程或其他类似方法；
* 开机自运行需自行添加到计划任务或者修改注册表等；

## License

MIT