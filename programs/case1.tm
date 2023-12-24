; the finite set of states
#Q = {check1,check2,check3,move_to_right,erase_to_left,write_reject,write_reject1,write_reject2,write_reject3,write_reject4,write_reject5,write_reject6,write_reject7,write_reject8,write_reject9,write_reject10,write_reject11,write_reject12,halt_reject,reset1,set_pointers1,set_pointers2,set_pointers3,set_pointers4,start,carry,find_last_b,found_last_b,halt_accept,find_first_c,erase_all}

; the finite set of input symbols
#S = {a,b}

; the complete set of tape symbols
#G = {0,1,_,c,I,l,e,g,a,l,n,p,u,t,x}

; the start state
#q0 = check1

; the blank symbol
#B = _

; the set of final states
#F = {start}

; the number of tapes
#N = 3

check1 a__ a__ r** check2
check1 ___ ___ *** write_reject; empty string
check1 b__ b__ *** move_to_right
check2 a__ a__ r** check2
check2 b__ b__ r** check3
check2 ___ ___ l** move_to_right; lack b, here we need to move left, make the head on the last character
check3 b__ b__ r** check3
check3 a__ a__ *** move_to_right
check3 ___ ___ l** reset1

;move the to the end(right direction) of the input string, but not erase
move_to_right *__ *__ r** move_to_right
move_to_right ___ ___ l** erase_to_left;  here we need to move left, make the head on the last character
;移动到字符的最左处
erase_to_left *__ ___ l** erase_to_left
erase_to_left ___ ___ r** write_reject
write_reject ___ I__ r** write_reject1
write_reject1 ___ l__ r** write_reject2
write_reject2 ___ l__ r** write_reject3
write_reject3 ___ e__ r** write_reject4
write_reject4 ___ g__ r** write_reject5
write_reject5 ___ a__ r** write_reject6
write_reject6 ___ l__ r** write_reject7
write_reject7 ___ ___ r** write_reject8
write_reject8 ___ I__ r** write_reject9
write_reject9 ___ u__ r** write_reject10
write_reject10 ___ p__ r** write_reject11
write_reject11 ___ u__ r** write_reject12
write_reject12 ___ t__ r** halt_reject

;reset to the left, set pointers
reset1 *__ *__ l** reset1
reset1 ___ ___ r** set_pointers1
set_pointers1 a__ aa_ rrr set_pointers1
set_pointers1 b__ b__ lll set_pointers2
set_pointers2 aa_ aa_ r*r set_pointers3
set_pointers3 ba_ bab r*r set_pointers3
set_pointers3 _a_ _a_ l*l set_pointers4
set_pointers4 bab bab r** start

start _ab cab r*l start
start _a_ ca_ rl* carry;进入进位，先检查a是否为_，是则结束，把第三条纸带的head挪到最后一个b的前一个位置
carry ___ ___ l** find_first_c
carry _a_ _a_ **r find_last_b;将纸带3的head找到最后一个b
find_last_b _ab _ab **r find_last_b
find_last_b _a_ _a_ **l found_last_b
found_last_b _ab _ab **l start
find_first_c c__ c__ l** find_first_c
find_first_c b__ b__ r** erase_all
erase_all *__ ___ l** erase_all
erase_all ___ ___ *** halt_accept











































