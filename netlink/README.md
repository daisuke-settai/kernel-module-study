# NETLINK テスト
カーネルモジュール - ユーザープログラム間でネットリンクソケットを作成し通信する． 
  
user-programにおけるソケット作成時の定数は linux/netlink.h を参照し使用していない定数を利用．  
22 ~ 31

### user-program.c
通常のソケットプログラミングと同様にプログラム可能．  
通信対象の指定はソケット作成時の第３引数で指定．

### kernel-program.c
netlink.h を参考にプログラム．  
netlink_kernel_create で通信経路確立．  
第一引数は"&init_net"．  
第二引数はuser-programでソケット作成時に指定した値．  
第三引数は"netlink_kernel_cfg"．  
  
netlink_kernel_cfg.input に関数を登録することで，パケット受信時に関数を実行できる．  
この関数の引数に受信データがsk_buffの状態で入る．   
送信データはnlmsghdrに格納．  
nlmsg_unicastでデータ送信
### out.txt
実行結果
