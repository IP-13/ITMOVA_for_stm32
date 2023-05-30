package com.ip13;

import com.ip13.compiler.SuperClass;

public class Main {
    public static void main(String[] args) throws Exception {
        String program = """
                int_equal(A: int, B: int): bool
                START
                  RETURN and(not(greater(A, B)), not(less(A, B)));
                FINISH
                                
                str_equal(S1:string, S2:string):bool
                START
                  RETURN and(int_equal(like(S1, S2), 0), int_equal(length(S1), length(S2)));
                FINISH
                                
                MAIN
                S1:string;
                S2:string;
                assign(S1, "123456789");
                assign(S2, "987654321");
                print(str_equal(S1, S2));
                """;

        SuperClass.generateFileForCLabExecution(program);
    }
}

