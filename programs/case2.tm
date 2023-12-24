; the finite set of states
#Q = {reject,accept,check,check1,find_first_c,copy_to_end,pointers_to_left,set_pointer2,start,write_true,write_true1,write_true2,write_true3,write_false,write_false1,write_false2,write_false3,write_false4,halt,erase_a,erase_r}

; the finite set of input symbols
#S = {a,b,c}

; the complete set of tape symbols
#G = {a,b,c,_,t,r,u,e,f,l,s}

; the start state
#q0 = check

; the blank symbol
#B = _

; the set of final states
#F = {write_true}

; the number of tapes
#N = 2

; the transition functions

check __ __ ** write_true
check c_ c_ r* check1
check1 __ __ l* accept
check a_ a_ ** find_first_c
check b_ b_ ** find_first_c

find_first_c a_ aa rr find_first_c
find_first_c b_ bb rr find_first_c
find_first_c c_ cc rr copy_to_end
find_first_c __ __ l* reject;can not find c

;关键，第一个c前面只有ab，后面只有abc
copy_to_end a_ aa rr copy_to_end
copy_to_end b_ bb rr copy_to_end
copy_to_end c_ cc rr copy_to_end
copy_to_end __ __ ll pointers_to_left
pointers_to_left ** ** ll pointers_to_left
pointers_to_left __ __ rr set_pointer2
set_pointer2 *a *a *r set_pointer2
set_pointer2 *b *b *r set_pointer2
set_pointer2 *c *c *r start

;开始比较
;accept或reject后,统一把第一条纸带变成非_，第二条纸带写成_,方便写结果
start aa _a rr start
start bb _b rr start
start ab a_ ** reject
start ac a_ ** reject
start ba b_ ** reject
start bc b_ ** reject
start c_ c_ ** accept
start c* c_ ** reject

;写结果,先找到最左，再删除到最右，再写true or false
accept *_ *_ l* accept
accept __ __ r* erase_a
erase_a *_ __ r* erase_a
erase_a __ __ l* write_true

reject *_ *_ l* reject
reject __ __ r* erase_r
erase_r *_ __ r* erase_r
erase_r __ __ l* write_false

write_true __ t_ r* write_true1
write_true1 __ r_ r* write_true2
write_true2 __ u_ r* write_true3
write_true3 __ e_ r* halt

write_false __ f_ r* write_false1
write_false1 __ a_ r* write_false2
write_false2 __ l_ r* write_false3
write_false3 __ s_ r* write_false4
write_false4 __ e_ r* halt

