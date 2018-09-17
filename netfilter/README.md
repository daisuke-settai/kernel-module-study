# NETFILTER テスト
カーネルネットワークスタック内に自作関数を定義しパケット処理が可能な仕組みを試す．  
ネットワークスタック内に置いて，  
1. PRE_PROUTING
2. LOCAL_IN
3. FORWARDING
4. LOCAL_OUT 
5. POST_ROUTING

の5箇所のタイミングで関数処理登録可能なフックが存在する．   
また，プロトコルを指定して，指定プロトコルのパケットにのみ関数が実行されるようにもできる．    
登録関数実行の優先順位も指定可能(値が小さいほど優先度高).   

nf_register_hook: 関数の登録を行う．    
struct nf_hook_ops: 関数登録の引数．各種設定値を格納    
hookfn: 登録できる関数, 
```
typedef unsigned int nf_hookfn(void *priv,      //nf_hook_opsのprivが格納.任意の値を使える．
             struct sk_buff *skb,               //受信パケット
             const struct nf_hook_state *state);//
```

# 環境
linux-kernel-4.4.0

### netfilter_test.c
netfilter に2つの関数を登録．   
同一フック，同一プロトコルに，同じ優先度で異なる関数を登録．  
登録順に実行．  
1つ目の関数で'192.168.123.11'からきたパケットのみsrc addrを'192.168.123.2'に書き換え．  
2つ目の関数で書き換えができているかprintkにて確認． 

### netfilter_priv_test.c
nf_hook_ops のprivを利用．  
hookfn に値が渡されていることを確認． 
  
### netfilter_test2.c
nf_hook_ops.pf をARPにして動作確認  
arpingコマンドを用いて確認  

### netfilter_check_max_register.c
netfilter に登録できる関数の数に上限があるのか確認する．    
コードを見た感じ，関数登録の際，上限を設定する処理は見当たらないが一応チェック．  
N個の関数を登録し確認.   
N: 70,000 => ok   
N: 1,000,000 => ok (register:10分以上かかった)    
  -> 実行結果の一部をout.txtに記載  
