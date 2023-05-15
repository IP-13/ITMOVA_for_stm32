package com.ip13.compiler;

public enum ByteCodeCommands {
    // program flow
    exit("0"),
    jmp("1"),
    jdec("2"),
    jret("3"),
    // from loop
    loop("10"),
    fint("11"),
    fvar("12"),
    // if operator
    jt("20"),
    // func-related
    call("100"),
    lit("110"),
    var("111"),
    ofc("112"), // other function call, TODO come up with a better name
    rlit("120"),
    rvar("121"),
    rofc("122"),
    // types
    BLN("200"),
    INT("201"),
    FLT("202"),
    STR("203"),
    PRT("204"),
    VOID("205"),
    // itmova-core
    print("700"),
    assign("701"),
    // dynamic memory
    get_data("710"),
    set_data("711"),
    get_addr("712"),
    malloc("713"),
    // bool functions
    and("720"),
    or("721"),
    not("722"),
    // str functions
    concat("725"),
    substr("726"),
    like("727"),
    length("728"),
    // math unary
    abs("735"),
    sin("736"),
    cos("737"),
    inc("738"),
    dec("739"),
    // math binary
    log("755"),
    pow("756"),
    sum("757"),
    sub("758"),
    mul("759"),
    div("760"),
    mod("761"),
    min("762"),
    max("763"),
    // math non-arg
    PI("775"),
    E("776"),
    random("777");


    private final String numberFormat;


    ByteCodeCommands(String numberFormat) {
        this.numberFormat = numberFormat;
    }


    public String getNumberFormat() {
        return numberFormat;
    }


    public static String getItmovaCore(String s) {
        try {
            return ByteCodeCommands.valueOf(s).toString();
        } catch (IllegalArgumentException ex) {
            return null;
        }
    }
}