# Preface

Developing applications in a logic programming language is subject to the same issues as that face developers in other programming languages: programming is still a team sport; and safety is nearly always critical. L&O is a logic programming language that is oriented to development of complex applications that must be similarly safe and effective.

## Software Engineering

Developing any kind of software is a complex task, made more difficult by the possibilities and risks offered by the Internet. Such complexity is not removed simply by adopting logic as the foundation of one’s languages. Integration, reliability, security, modularization, evolvability, versioning, safety are all qualities that are important for software systems that are independent of the underlying technology. For knowledge intensive applications the list continues – we also require flexibility, explainability, awareness of context. The design of L&O is guided by a strong desire to gain the benefits of modern software engineering best practice as well as that of knowledge engineering.

L&O is a *multi-paradigm* language – it has a strong foundation in object oriented programming, functional programming and logic programming. In addition, it is a multi-threaded language with communication capabilities. This is a powerful combination aiming to solve the hard issues of complex software development.

## Static types

L&O is a strongly statically typed programming language. The purpose of using a static type system is to enhance programmers’ confidence in the correctness of the program – it cannot replace a formal proof of correctness.

The type system of L&O is quite rich and expressive; this is intentional -- in order to reduce the _temptation_ to step outside the strict confines of type safety. Some of the features of L&O types include:

* Higher-order type terms that can denote function and other program values
* Universally and existentially quantified types
* Contracts and implementations
* Structural and nominative type terms


For example, the L&O version of `find` above really needs a *type annotation* before it is complete:

	find: all u,t ~~ searchable[u] |: (u,t){}.

where we both give an explicit (quantified) type and require that the entity being search is `searchable`.

We use an approach based on Hindley & Milner’s @hindley:69 type term approach for representing types. However, type unification is augmented with a sub-type relation – permitting types and classes to be defined as extensions of other types. In addition we require all programs and top-level variables and constants to have explicit type declarations. Variables in rules do not need type declarations – although they are permitted.

Having a static typed language can be quite constrictive compared to the untyped freedom one gets in languages such as **Prolog**. However, for applications requiring a strong sense of reliability, having a strongly typed language provides a better base than an untyped language.

In addition to being statically typed, the reader might have noticed that the `find` program we introduced above had a *type annotation*. We believe that explicitly annotating program types is an effective compromise between declaring the type of *every* variable and declaring none of them.

## Object orientation

L&O’s *object orderedness* is fundamental to L&O’s approach to software engineering. To support the evolution of programs it is important to be able to modify a large program in a way that has manageable and predictable consequences. This is aided by having a clean separation between interfaces and implementations of components of the program. Being able to change the implementation of a component without changing all the *references* to the program is a basic benefit of object ordered programming.

Similarly, any change to a component (whether code or data) should not require changes to unaffected parts of the overall system. For example, merely *adding* a new function to a module should not require modifying programs that only use existing features of the module. Both of these are important benefits of object ordered programming

> We use the term *object ordered programming* to avoid some of the specifics of common object oriented languages – the key feature of object ordered programming is the encapsulation of code and data that permits the *hiding* of the implementation of a concept from the parts of the application that wish to merely *use* the concept. Features such as inheritance are important but secondary compared to the core concepts of encapsulation and hiding.

As an example of the importance of interfaces, consider representing binary trees using **Prolog** terms – which is **Prolog**’s basic means for structuring dynamic data. For example, we might use a `tree` term:

	tree(empty,"A",tree(empty,"B",tree(empty,"C",empty)))

to denote a basic binary tree structure containing the strings `"A"`, `"B"` and `"C"`.

As a data structuring technique, the **Prolog** term is versatile and simple; however, it combines *implementation* of data structures with *access* in an unfortunate way. For example, to search a `tree` for an element we must use specific `tree` term patterns to unify against the actual tree:

	find(A,tree(_,A,_)).
	find(A,tree(L,B,_)) :- A<B, find(A,L).
	find(A,tree(_,B,R)) :- A>B, find(A,R).

This simple program can be used to search an ordered binary tree, looking for elements that unify with the search term. The `find` program is concise, relatively clear and efficient. Perhaps this program was exactly what was needed.

However, should it become necessary to adjust our tree representation – perhaps to include a weight element – then, in **Prolog**, *all* references to the `tree` term will need to change, including existing uses which have no interest in the new weight feature. Our `find` program will certainly have to be modified – to add the extra argument to the `tree` term and perhaps ignore it. On average there will an order of magnitude more references that *use* the concept of `tree` than references which *define* the essence of `tree`.

In L&O we can write the `find` program in a way that does not depend on the shape of the `tree` term:

	find(A,T) :- T.hasLabel(A).
	find(A,T) :- A<T.label(), find(A,T.left()).
	find(A,T) :- A>T.label(), find(A,T.right()).

L&O’s labeled theory notation makes it straightforward to encapsulate the `tree` concept in an object, and to use an interface contract to access the tree. As a result, we should be able to add weights to our tree without upsetting existing uses of the tree – in particular, the `find` program does not need to be modified.\note{It may need to be re-compiled however.}

For an OO language, such a capability is not novel, but traditionally, logic programming languages have not really focused on such engineering issues.

L&O has some features that distinguish it from some OO languages such as Java\tm. L&O’s object notation is based on Logic and Objects (@fgm:92) with some significant simplifications and modifications to incorporate types and *interface*s.

## Ontologies

Since L&O is a Logic Programming language, it makes sense to ask how suitable it is for developing Ontologies.

## Meta-order and object-order

Unlike **Prolog**, L&O does not permit data to be directly interpreted as code. The standard **Prolog** approach of using the same language for meta-level names of programs and programs themselves – which in turn allows program text to be manipulated like other data – has a number of technical problems; especially when considering distributed and secure applications. However, as we shall see, L&O’s object oriented features allow us to emulate the important uses of Prolog’s meta-level features. Moreover, in L&O we can do this in a type safe way.

## Threads and distribution

L&O is a multi-tasking programming language. It is possible to spawn off computations as separate threads or tasks.

In a multi-threaded environment there are two overlapping concerns – sharing of resources and coordination of activities. In the cases of shared resources the primary requirement is that the different users of a resource see consistent views of the resource. In the case of coordination the primary requirement is a means of controlling the flow of execution in the different threads of activity.

L&O threads may share access to objects, and to their state in the case of stateful objects – within a single invocation of the system. L&O supports synchronized access to such shared objects to permit contention issues to be addressed.

Synchronizing access to shared resources is important, however it is not sufficient to achieve coordination between concurrent activities. In L&O, coordination is achieved through *message communication*. Our message communication functionality is not built-in to the language but is made available via the use of standard *packages*, in particular the `go.mbox` package. The communications model in `go.mbox` is very simple: there are mailboxes and dropboxes: threads read their mail by querying their mailbox objects and can send messages to other threads using dropboxes linked to mailboxes. The model permits multiple implementations of the concept, indeed a single mailbox might receive mail delivered from a variety of kinds of dropboxes.

Overall, the intention behind the design of L&O is to make programs more transparent: what you see in a L&O program is what you mean – there can be no hidden semantics. This property is what makes L&O a reasonable language to use for high integrity applications – such as agents that will be performing tasks that may involve real resources, or in safety critical areas.


##### Multiple paradigms

L&O is a multi-paradigm language – there are specialized notations for functions, action rules and grammar rules, as well as predicates. The reason for this is two fold: it allows us to have tailored syntax and semantics for the different kinds of programs being written. Secondly, by offering these different notations we can encapsulate a more suitable semantics without the use of ugly operators such as **Prolog**’s cut operator.

In many cases a logic programmer knows full well whether their program is intended to be fully relational or is actually a function. By giving different notations for these cases it allows programmers to signal their intentions more clearly than if all kinds of program have to be expressed in the single formalism of a logic clause.

##### The structure of this book

This book is intended to be read as an introduction to the language, to using it and to show how sophisticated programs can be built using its facilities. We assume some familiarity with programming, especially logic programming, although the pace is relatively gentle.

Part I gets us started, with the style of L&O programs. Part II explores some programming examples and techniques. In Part III we look at the features of L&O in more detail. This book is not a reference manual for L&amp;O; however, we do aim to cover the language in sufficient detail to impart the essence of L&O.

Our style in presenting L&O is rather informal; this is deliberate – our focus is to explain the shape of L&O and to show its utility. For a more detailed and formal explanation of the features of L&O the reader is referred to the L&O reference manual.

##### Conventions

We use a `typewriter` font whenever we are quoting either a specific element of L&O, or something that may be typed in at the keyboard. We use *emphasis* both for emphasis and to introduce a term for the first time. Many terms are defined in the Glossary, see page \pageref{glossary}.

> Every so often – sometimes more than once on a given page – you will see a paragraph highlighted this way. Such notes are often comments about how to use a given bit of information, or may be a warning about some issue that is relevant to the text at hand.

##### Getting and installing L&O

[Appendix Install][Install] discusses the process involved in installing the L&O system. Please note that this process is quite likely to evolve; however, the location:

	git@github.com:fmccabe/l-and-o.git

will remain as a good source to get the L&O system.

##### Acknowledgments

Creating a programming language and writing about it are not solo activities. In my case I had the substantial help of a number of colleagues; most notably Keith Clark who is really a part of the design team for L&O. He has tracked L&O’s progress through many different versions.

Users are critical in any software enterprise, and Johnny Knottenbelt was one of the first. I hope that the inevitable stream of bugs that he found did not cause him too much grief.

In addition Jonathan Dale and Kevin Twidle who have endured many *explanations* of this or that new feature while trying to do their proper work. I thank my family, Midori and Stephen who have had less of a husband/father than they have the right to expect.

Finally, I thank you, dear reader, for taking the trouble to parse my English and following me on the road to L&O.

##### Contact

If you have any questions, about the L&O language or this book, please contact me at `frankmccabe@mac.com`.
