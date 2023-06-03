import sys

def gen_tabs(tabs: int):

    return "\t" * tabs

def gen_ops_statement(lhs: int, rhs: int, obj: str, ops: str, ops_, tabs: int) -> list[str]:

    rs = ["%sassert(((%s(\"%s\") %s %s(\"%s\")) == %s(\"%s\")));" % (gen_tabs(tabs), obj, bin(lhs), ops, obj, bin(rhs), obj, bin(ops_(lhs, rhs))),
          "%sassert(((%s(\"%s\") %s %s) == %s(\"%s\")));" % (gen_tabs(tabs), obj, bin(lhs), ops, rhs, obj, bin(ops_(lhs, rhs)))]
        
    return rs

def plus(lhs: int, rhs: int):

    return lhs + rhs

def minus(lhs: int, rhs: int):

    return lhs - rhs

def mul(lhs: int, rhs: int):

    return lhs * rhs

def div(lhs: int, rhs: int):

    return lhs // rhs

def and_ops(lhs: int, rhs: int):

    return lhs & rhs

def or_ops(lhs: int, rhs: int):

    return lhs | rhs 

def xor_ops(lhs: int, rhs: int):

    return lhs ^ rhs 

def lshift(lhs: int, rhs: int):

    return lhs << rhs 

def rshift(lhs: int, rhs: int):

    return lhs >> rhs 

def mod_ops(lhs: int, rhs: int):

    return lhs % rhs 

def main():

    op_file = sys.argv[1] 
    bound = int(sys.argv[2])
    objs = sys.argv[3:]

    rs = []

    for obj in objs:

        for i in range(bound):

            for j in range(bound):

                rs += gen_ops_statement(i, j, obj, "+", plus, 1)
                rs += gen_ops_statement(i, j, obj, "*", mul, 1)
                rs += gen_ops_statement(i, j, obj, "&", and_ops, 1)
                rs += gen_ops_statement(i, j, obj, "^", xor_ops, 1)
                rs += gen_ops_statement(i, j, obj, "|", or_ops, 1)
                rs += gen_ops_statement(i, j, obj, "<<", lshift, 1)
                rs += gen_ops_statement(i, j, obj, ">>", rshift, 1)

                rs += gen_ops_statement(i, j, obj, "+=", plus, 1)
                rs += gen_ops_statement(i, j, obj, "*=", mul, 1)
                rs += gen_ops_statement(i, j, obj, "&=", and_ops, 1)
                rs += gen_ops_statement(i, j, obj, "^=", xor_ops, 1)
                rs += gen_ops_statement(i, j, obj, "|=", or_ops, 1)
                rs += gen_ops_statement(i, j, obj, "<<=", lshift, 1)
                rs += gen_ops_statement(i, j, obj, ">>=", rshift, 1)

    for obj in objs:

        for i in range(bound):

            rs += gen_ops_statement(i, 0, obj, "-", minus, 1)
            rs += gen_ops_statement(i, 0, obj, "-=", minus, 1)

            for j in range(1, i):

                rs += gen_ops_statement(i, j, obj, "-", minus, 1)
                rs += gen_ops_statement(i, j, obj, "/", div, 1)
                rs += gen_ops_statement(i, j, obj, "%", mod_ops, 1)
                rs += gen_ops_statement(i, j, obj, "-=", minus, 1)
                rs += gen_ops_statement(i, j, obj, "/=", div, 1)
                rs += gen_ops_statement(i, j, obj, "%=", mod_ops, 1)


    content = "\n".join(["#define BIGNUM_TEST",
                         "#include <iostream>",
                         "#include \"BigNum.h\"",
                         "using namespace bignum::integer::usgn;",
                         "using namespace dgstd;",
                         "int main(){",
                         "\tResourceInitiator(1);",
                         "\n".join(rs),
                         "\tResourceDestructor();",
                         "\tstd::cout << \"completed\";",
                         "}"])
    
    with open(op_file, "w") as f:

        f.write(content)

main()