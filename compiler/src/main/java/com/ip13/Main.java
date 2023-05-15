package com.ip13;

import com.ip13.antlr.ItmovaBaseListener;
import com.ip13.antlr.ItmovaLexer;
import com.ip13.antlr.ItmovaParser;
import com.ip13.compiler.SuperClass;
import org.antlr.v4.runtime.CharStreams;
import org.antlr.v4.runtime.CommonTokenStream;
import org.antlr.v4.runtime.tree.ParseTree;
import org.antlr.v4.runtime.tree.ParseTreeWalker;


public class Main {
    public static void main(String[] args) throws Exception {
        String program = """
                my_func(A: int) : pointer   
                START
                FROM 1 TO 5 WITH 2:
                START
               
                B : int;
                C : string;
                
                IF and(FALSE, TRUE):
                START
                sum(B, C);
                FINISH
                
                FINISH
                RETURN A;
                FINISH  
                
                
                my_func2(A : int) : void 
                START
                sum(A, 4);
                FINISH
               
                
                                
                MAIN
                A : int;
                my_func2(A);
                """;

        ItmovaLexer lexer = new ItmovaLexer(CharStreams.fromString(program));
        CommonTokenStream tokens = new CommonTokenStream(lexer);
        ItmovaParser parser = new ItmovaParser(tokens);
        ParseTree tree = parser.program();
        ParseTreeWalker walker = new ParseTreeWalker();
        walker.walk(new ItmovaBaseListener(), tree);

        SuperClass.showFuncList();
        System.out.println("__________________________________");
        SuperClass.showByteCode();
    }
}
