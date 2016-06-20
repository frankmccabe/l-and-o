/* Automatically generated, do not edit */

:-module(operators,[infixOp/4,prefixOp/3,postfixOp/3,isOperator/2,follows/3,final/2]).

  infixOp(". ",1899,1900,1900).	 /* statement separator */
  infixOp("::=",1459,1460,1459).	 /* user type definition */
  infixOp("|",1249,1250,1250).	 /* type union and disjunction */
  infixOp(":",1249,1250,1249).	 /* type annotation */
  infixOp("~~",1239,1240,1239).	 /* quantifier */
  infixOp("?",1199,1200,1199).	 /* conditional operator */
  infixOp("<=>",1199,1200,1199).	 /* class constructor type */
  infixOp("=>",1199,1200,1199).	 /* function arrow */
  infixOp(":-",1199,1200,1199).	 /* clause arrow */
  infixOp(":--",1199,1200,1199).	 /* strong clause */
  infixOp("-->",1199,1200,1199).	 /* grammar rule */
  infixOp("*>",1151,1152,1151).	 /* all solutions */
  infixOp("::",1129,1125,1129).	 /* guard marker */
  infixOp("||",1059,1060,1059).	 /* bag of constructor */
  infixOp(",",999,1000,1000).	 /* tupling, conjunction */
  infixOp(",..",999,1000,1000).	 /* list cons */
  infixOp("<=",949,950,949).	 /* class rule arrow */
  infixOp("<~",949,949,948).	 /* type interface rule */
  infixOp("=",899,900,899).	 /* variable declaration */
  infixOp("==",899,900,899).	 /* equality predicate */
  infixOp("\\=",899,900,899).	 /* not unifyable */
  infixOp("!=",899,900,899).	 /* not equal */
  infixOp("<",899,900,899).	 /* less than */
  infixOp("=<",899,900,899).	 /* less than or equal */
  infixOp(">",899,900,899).	 /* greater than */
  infixOp(">=",899,900,899).	 /* greater than or equal */
  infixOp(".=",899,900,899).	 /* match predicate */
  infixOp("=.",899,900,899).	 /* match predicate */
  infixOp("..",895,896,895).	 /* class body */
  infixOp("in",894,895,894).	 /* set membership */
  infixOp("<>",799,800,800).	 /* list append */
  infixOp("#",759,760,759).	 /* package separator */
  infixOp("+",720,720,719).	 /* addition */
  infixOp("-",720,720,719).	 /* subtraction */
  infixOp("*",700,700,699).	 /* multiplication */
  infixOp("/",700,700,699).	 /* division */
  infixOp("%",700,700,699).	 /* quotient */
  infixOp("rem",700,700,699).	 /* remainder function */
  infixOp("**",600,600,599).	 /* exponentiation */
  infixOp("%%",499,500,499).	 /* grammar parse */
  infixOp("^",499,500,499).	 /* grammar iterator */
  infixOp("~",934,935,934).	 /* grammar remainder */
  infixOp(".",450,450,449).	 /* object access */
  prefixOp("private",1700,1699).	 /* private program */
  prefixOp("public",1700,1699).	 /* public program */
  prefixOp("assert",1260,1259).	 /* assert condition */
  prefixOp("import",900,899).	 /* import module */
  prefixOp("all",1245,1244).	 /* universal quantifier */
  prefixOp("\\+",905,904).	 /* logical negation */
  prefixOp("@",905,904).	 /* tau pattern */
  prefixOp("-",300,299).	 /* arithmetic negation */
  postfixOp(". ",1899,1900).	 /* statement terminator */
  postfixOp(";",1149,1150).	 /* action terminator */
  postfixOp("+",759,760).	 /* input mode */
  postfixOp("-",759,760).	 /* output mode */
  postfixOp("-+",759,760).	 /* bidirectional mode */
  postfixOp("+-",759,760).	 /* bidirectional mode */
  postfixOp("!",904,905).	 /* one solution operator */
  /* Define isOperator */
  isOperator(Op,Pr) :- prefixOp(Op,Pr,_).
  isOperator(Op,Pr) :- infixOp(Op,_,Pr,_).
  isOperator(Op,Pr) :- postfixOp(Op,_,Pr).
  follows('','%','%').
  follows('%','%','%%').
  follows('','*','*').
  follows('*','*','**').
  follows('*','>','*>').
  follows('','+','+').
  follows('+','-','+-').
  follows('',',',',').
  follows(',','.',',.').
  follows(',.','.',',..').
  follows('','-','-').
  follows('-','+','-+').
  follows('-','-','--').
  follows('--','>','-->').
  follows('','.','.').
  follows('.','.','..').
  follows('.',' ','. ').
  follows('.','=','.=').
  follows('','/','/').
  follows('','|','|').
  follows('|','|','||').
  follows('','~','~').
  follows('~','~','~~').
  follows('','\\','\\').
  follows('\\','+','\\+').
  follows('\\','=','\\=').
  follows('','^','^').
  follows('',':',':').
  follows(':',':','::').
  follows('::','=','::=').
  follows(':','-',':-').
  follows(':-','-',':--').
  follows('',';',';').
  follows('','<','<').
  follows('<','~','<~').
  follows('<','=','<=').
  follows('<=','>','<=>').
  follows('<','>','<>').
  follows('','=','=').
  follows('=','.','=.').
  follows('=','<','=<').
  follows('=','=','==').
  follows('=','>','=>').
  follows('','>','>').
  follows('>','=','>=').
  follows('','?','?').
  follows('','@','@').
  follows('','!','!').
  follows('!','=','!=').
  follows('','#','#').
  final('%',"%").	 /* quotient */
  final('%%',"%%").	 /* grammar parse */
  final('*',"*").	 /* multiplication */
  final('**',"**").	 /* exponentiation */
  final('*>',"*>").	 /* all solutions */
  final('+',"+").	 /* input mode */
  final('+-',"+-").	 /* bidirectional mode */
  final(',',",").	 /* tupling, conjunction */
  final(',..',",..").	 /* list cons */
  final('-',"-").	 /* output mode */
  final('-+',"-+").	 /* bidirectional mode */
  final('-->',"-->").	 /* grammar rule */
  final('.',".").	 /* object access */
  final('..',"..").	 /* class body */
  final('. ',". ").	 /* statement terminator */
  final('.=',".=").	 /* match predicate */
  final('/',"/").	 /* division */
  final('|',"|").	 /* type union and disjunction */
  final('||',"||").	 /* bag of constructor */
  final('~',"~").	 /* grammar remainder */
  final('~~',"~~").	 /* quantifier */
  final('\\+',"\\+").	 /* logical negation */
  final('\\=',"\\=").	 /* not unifyable */
  final('^',"^").	 /* grammar iterator */
  final(':',":").	 /* type annotation */
  final('::',"::").	 /* guard marker */
  final('::=',"::=").	 /* user type definition */
  final(':-',":-").	 /* clause arrow */
  final(':--',":--").	 /* strong clause */
  final(';',";").	 /* action terminator */
  final('<',"<").	 /* less than */
  final('<~',"<~").	 /* type interface rule */
  final('<=',"<=").	 /* class rule arrow */
  final('<=>',"<=>").	 /* class constructor type */
  final('<>',"<>").	 /* list append */
  final('=',"=").	 /* variable declaration */
  final('=.',"=.").	 /* match predicate */
  final('=<',"=<").	 /* less than or equal */
  final('==',"==").	 /* equality predicate */
  final('=>',"=>").	 /* function arrow */
  final('>',">").	 /* greater than */
  final('>=',">=").	 /* greater than or equal */
  final('?',"?").	 /* conditional operator */
  final('@',"@").	 /* tau pattern */
  final('!',"!").	 /* one solution operator */
  final('!=',"!=").	 /* not equal */
  final('#',"#").	 /* package separator */
