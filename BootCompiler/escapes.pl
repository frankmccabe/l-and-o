/* Automatically generated, do not edit */

:-module(escapes,[isEscape/1,escapeType/2]).

escapeType("exit",funType([type("lo.arith*integer")],voidType)).
escapeType("_command_line",funType([],typeExp("lo.list*list",[type("lo.string*string")]))).
escapeType("_command_opts",funType([],typeExp("lo.list*list",[tupleType([type("lo.string*string"),type("lo.string*string")])]))).
escapeType("_unify",univType(kVar("t"),predType([kVar("t"),kVar("t")]))).
escapeType("_identical",univType(kVar("t"),predType([kVar("t"),kVar("t")]))).
escapeType("_match",univType(kVar("t"),predType([kVar("t"),kVar("t")]))).
escapeType("var",univType(kVar("t"),predType([kVar("t")]))).
escapeType("nonvar",univType(kVar("t"),predType([kVar("t")]))).
escapeType("_int_plus",funType([type("lo.arith*integer"),type("lo.arith*integer")],type("lo.arith*integer"))).
escapeType("_int_minus",funType([type("lo.arith*integer"),type("lo.arith*integer")],type("lo.arith*integer"))).
escapeType("_int_times",funType([type("lo.arith*integer"),type("lo.arith*integer")],type("lo.arith*integer"))).
escapeType("_int_div",funType([type("lo.arith*integer"),type("lo.arith*integer")],type("lo.arith*integer"))).
escapeType("_flt_plus",funType([type("lo.arith*float"),type("lo.arith*float")],type("lo.arith*float"))).
escapeType("_flt_minus",funType([type("lo.arith*float"),type("lo.arith*float")],type("lo.arith*float"))).
escapeType("_flt_times",funType([type("lo.arith*float"),type("lo.arith*float")],type("lo.arith*float"))).
escapeType("_flt_div",funType([type("lo.arith*float"),type("lo.arith*float")],type("lo.arith*float"))).
escapeType("_int_abs",funType([type("lo.arith*integer")],type("lo.arith*integer"))).
escapeType("_flt_abs",funType([type("lo.arith*float")],type("lo.arith*float"))).
escapeType("_isCcChar",predType([type("lo.arith*integer")])).
escapeType("_isCfChar",predType([type("lo.arith*integer")])).
escapeType("_isCnChar",predType([type("lo.arith*integer")])).
escapeType("_isCoChar",predType([type("lo.arith*integer")])).
escapeType("_isCsChar",predType([type("lo.arith*integer")])).
escapeType("_isLlChar",predType([type("lo.arith*integer")])).
escapeType("_isLmChar",predType([type("lo.arith*integer")])).
escapeType("_isLoChar",predType([type("lo.arith*integer")])).
escapeType("_isLtChar",predType([type("lo.arith*integer")])).
escapeType("_isLuChar",predType([type("lo.arith*integer")])).
escapeType("_isMcChar",predType([type("lo.arith*integer")])).
escapeType("_isMeChar",predType([type("lo.arith*integer")])).
escapeType("_isMnChar",predType([type("lo.arith*integer")])).
escapeType("_isNdChar",predType([type("lo.arith*integer")])).
escapeType("_isNlChar",predType([type("lo.arith*integer")])).
escapeType("_isNoChar",predType([type("lo.arith*integer")])).
escapeType("_isPcChar",predType([type("lo.arith*integer")])).
escapeType("_isPdChar",predType([type("lo.arith*integer")])).
escapeType("_isPeChar",predType([type("lo.arith*integer")])).
escapeType("_isPfChar",predType([type("lo.arith*integer")])).
escapeType("_isPiChar",predType([type("lo.arith*integer")])).
escapeType("_isPoChar",predType([type("lo.arith*integer")])).
escapeType("_isPsChar",predType([type("lo.arith*integer")])).
escapeType("_isScChar",predType([type("lo.arith*integer")])).
escapeType("_isSkChar",predType([type("lo.arith*integer")])).
escapeType("_isSmChar",predType([type("lo.arith*integer")])).
escapeType("_isSoChar",predType([type("lo.arith*integer")])).
escapeType("_isZlChar",predType([type("lo.arith*integer")])).
escapeType("_isZpChar",predType([type("lo.arith*integer")])).
escapeType("_isZsChar",predType([type("lo.arith*integer")])).
escapeType("_isLetterChar",predType([type("lo.arith*integer")])).
escapeType("_digitCode",funType([type("lo.arith*integer")],type("lo.arith*integer"))).
escapeType("_int2str",funType([type("lo.arith*integer"),type("lo.arith*integer"),type("lo.arith*integer"),type("lo.arith*integer")],type("lo.string*string"))).
escapeType("_flt2str",funType([type("lo.arith*float"),type("lo.arith*integer"),type("lo.arith*integer"),type("lo.logical*logical"),type("lo.logical*logical")],type("lo.string*string"))).
escapeType("explode",funType([type("lo.string*string")],typeExp("lo.list*list",[type("lo.arith*integer")]))).
escapeType("implode",funType([typeExp("lo.list*list",[type("lo.arith*integer")])],type("lo.string*string"))).
isEscape("exit").
isEscape("_command_line").
isEscape("_command_opts").
isEscape("_unify").
isEscape("_identical").
isEscape("_match").
isEscape("var").
isEscape("nonvar").
isEscape("_int_plus").
isEscape("_int_minus").
isEscape("_int_times").
isEscape("_int_div").
isEscape("_flt_plus").
isEscape("_flt_minus").
isEscape("_flt_times").
isEscape("_flt_div").
isEscape("_int_abs").
isEscape("_flt_abs").
isEscape("_isCcChar").
isEscape("_isCfChar").
isEscape("_isCnChar").
isEscape("_isCoChar").
isEscape("_isCsChar").
isEscape("_isLlChar").
isEscape("_isLmChar").
isEscape("_isLoChar").
isEscape("_isLtChar").
isEscape("_isLuChar").
isEscape("_isMcChar").
isEscape("_isMeChar").
isEscape("_isMnChar").
isEscape("_isNdChar").
isEscape("_isNlChar").
isEscape("_isNoChar").
isEscape("_isPcChar").
isEscape("_isPdChar").
isEscape("_isPeChar").
isEscape("_isPfChar").
isEscape("_isPiChar").
isEscape("_isPoChar").
isEscape("_isPsChar").
isEscape("_isScChar").
isEscape("_isSkChar").
isEscape("_isSmChar").
isEscape("_isSoChar").
isEscape("_isZlChar").
isEscape("_isZpChar").
isEscape("_isZsChar").
isEscape("_isLetterChar").
isEscape("_digitCode").
isEscape("_int2str").
isEscape("_flt2str").
isEscape("explode").
isEscape("implode").
