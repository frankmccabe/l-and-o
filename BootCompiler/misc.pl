:-module(misc,[concat/3,flatten/2,segment/3,last/2,reverse/2,revconcat/3,is_member/2,
        merge/3,intersect/3,subtract/3,replace/4,
        collect/4,map/3,rfold/4,
        appStr/3,appInt/3,appFlt/3,appSym/3,appQuoted/4,genstr/2,
        subPath/4,pathSuffix/3,starts_with/2,ends_with/2,
        stringHash/3,hashSixtyFour/2]).

concat([],X,X).
concat([E|X],Y,[E|Z]) :- concat(X,Y,Z).

flatten([],[]).
flatten([F|R],Z):- concat(F,U,Z), flatten(R,U).

reverse(X,Y) :- reverse(X,[],Y).
reverse([],X,X).
reverse([E|R],X,Y) :- reverse(R,[E|X],Y).

revconcat(X,Y,Z) :-
  reverse(X,Y,Z).

segment(Str,Ch,Segments) :- split_string(Str,Ch,"",Segments).

last([El],El).
last([_|Rest],El) :- last(Rest,El).

is_member(X,[X|_]).
is_member(X,[_|Y]) :- is_member(X,Y).

merge([],X,X).
merge([E|X],Y,Z) :- 
  is_member(E,Y),!,
  merge(X,Y,Z).
merge([E|X],Y,Z) :-
  merge(X,[E|Y],Z).

replace([],_,El,[El]).
replace([E|X],E,Nw,[Nw|X]).
replace([E|X],K,Nw,[E|Y]) :-
  replace(X,K,Nw,Y).

subtract(_,[],[]).
subtract(E,[E|O],O).
subtract(E,[X|I],[X|O]) :-
  subtract(E,I,O).

intersect([],_,[]).
intersect([E|X],Y,[E|Z]) :- is_member(E,Y), intersect(X,Y,Z).
intersect([_|X],Y,Z) :- intersect(X,Y,Z).

collect([],_,[],[]) :- !.
collect([El|More],T,[El|Ok],Not) :-
  call(T,El),!,
  collect(More,T,Ok,Not).
collect([El|More],T,Ok,[El|Not]) :-
  collect(More,T,Ok,Not).

map([],_,[]).
map([E|L],F,[El|R]) :-
  call(F,E,El),
  map(L,F,R).

rfold([],_,S,S).
rfold([E|L],F,S,Sx) :-
  call(F,E,S,S0),!,
  rfold(L,F,S0,Sx).

appStr(Str,O,E) :- string_chars(Str,Chrs), concat(Chrs,E,O).

appQuoted(Str,Qt,O,E) :- appStr(Qt,O,O1), string_chars(Str,Chars), quoteConcat(Chars,O1,O2), appStr(Qt,O2,E).

quoteConcat([],O,O).
quoteConcat(['"'|More],['\\','"'|Out],Ox) :- quoteConcat(More,Out,Ox).
quoteConcat([''''|More],['\\',''''|Out],Ox) :- quoteConcat(More,Out,Ox).
quoteConcat(['\\'|More],['\\','\\'|Out],Ox) :- quoteConcat(More,Out,Ox).
quoteConcat([C|More],[C|Out],Ox) :- quoteConcat(More,Out,Ox).

appSym(Sym,O,E) :- atom_chars(Sym,Chrs), concat(Chrs,E,O).

appInt(Ix,O,E) :- number_string(Ix,Str), string_chars(Str,Chrs), concat(Chrs,E,O).

appFlt(Dx,O,Ox) :- number_string(Dx,Str), string_chars(Str,Chrs), concat(Chrs,Ox,O).

subPath(Path,Marker,Suffix,Name) :-
  sub_string(Path,_,_,After,Marker),
  (After=0, string_concat(Path,Suffix,Name) ; string_concat(Path,".",P0),string_concat(P0,Suffix,Name)).
subPath(Path,Marker,Suffix,Name) :-
  string_concat(Path,Marker,P0),
  string_concat(P0,Suffix,Name).

ends_with(String,Tail) :- 
  string_concat(_,Tail,String).

starts_with(String,Front) :-
  string_concat(Front,_,String).

pathSuffix(String,Marker,Tail) :-
  split_string(String,Marker,"",[_,Local]),
  split_string(Local,".","",Segs),
  last(Segs,Tail).
pathSuffix(String,_,String).

genstr(Prefix,S) :-
  gensym(Prefix,A),
  atom_string(A,S).

stringHash(H,Str,Hx) :-
  string_codes(Str,Codes),
  hashCodes(Codes,H,Hx).

hashCodes([],H,H).
hashCodes([C|More],H0,Hx) :-
  H1 is 47*H0+C,
  hashCodes(More,H1,Hx).

hashSixtyFour(H0,H) :-
  H is H0 mod (1<<63).
