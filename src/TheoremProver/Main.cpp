#include "Scope.hpp"
#include <FileSystem/FileSystem.hpp>
#include <FunctionT.hpp>
#include <Logger.hpp>

void Main(int, char**, MathLib::FileSystem&) {
    Scope scope = MathLib::MakeArray<MathLib::String>(
        "Stack"_M,
        "X"_M,
        "new"_M,
        "pop"_M,
        "top"_M,
        "push"_M,

        "popPushAxiom"_M,
        "topPushAxiom"_M,
        "popNewAxiom"_M
    );
    if (!scope.AddAxiom(new Term(Term::Type::TypeDeclaration,
        new Term(Term::Type::Symbol, scope.StringToSymbol("new"_M)),
        new Term(Term::Type::Symbol, scope.StringToSymbol("Stack"_M))
    ))) MathLib::Panic("Failed to add axiom to scope");
    if (!scope.AddAxiom(new Term(Term::Type::TypeDeclaration,
        new Term(Term::Type::Symbol, scope.StringToSymbol("pop"_M)),
        new Term(Term::Type::Abstraction,
            new Term(Term::Type::Symbol, scope.StringToSymbol("Stack"_M)),
            new Term(Term::Type::Symbol, scope.StringToSymbol("Stack"_M))
        )
    ))) MathLib::Panic("Failed to add axiom to scope");
    if (!scope.AddAxiom(new Term(Term::Type::TypeDeclaration,
        new Term(Term::Type::Symbol, scope.StringToSymbol("top"_M)),
        new Term(Term::Type::Abstraction,
            new Term(Term::Type::Symbol, scope.StringToSymbol("Stack"_M)),
            new Term(Term::Type::Symbol, scope.StringToSymbol("X"_M))
        )
    ))) MathLib::Panic("Failed to add axiom to scope");
    if (!scope.AddAxiom(new Term(Term::Type::TypeDeclaration,
        new Term(Term::Type::Symbol, scope.StringToSymbol("push"_M)),
        new Term(Term::Type::Abstraction,
            new Term(Term::Type::Symbol, scope.StringToSymbol("X"_M)),
            new Term(Term::Type::Abstraction,
                new Term(Term::Type::Symbol, scope.StringToSymbol("Stack"_M)),
                new Term(Term::Type::Symbol, scope.StringToSymbol("Stack"_M))
            )
        )
    ))) MathLib::Panic("Failed to add axiom to scope");
    if (!scope.AddAxiom(new Term(Term::Type::TypeDeclaration,
        new Term(Term::Type::Symbol, scope.StringToSymbol("popPushAxiom"_M)),
        new Term(Term::Type::Abstraction,
            new Term(Term::Type::Symbol, scope.StringToSymbol("X"_M)),
            new Term(Term::Type::Abstraction,
                new Term(Term::Type::Symbol, scope.StringToSymbol("Stack"_M)),
                new Term(Term::Type::Equivalence,
                    new Term(Term::Type::Application,
                        new Term(Term::Type::Symbol, scope.StringToSymbol("pop"_M)),
                        new Term(Term::Type::Application,
                            new Term(Term::Type::Application,
                                new Term(Term::Type::Symbol, scope.StringToSymbol("push"_M)),
                                new Term(Term::Type::Variable, 1)
                            ),
                            new Term(Term::Type::Variable, 0)
                        )
                    ),
                    new Term(Term::Type::Variable, 0)
                )
            )
        )
    ))) MathLib::Panic("Failed to add axiom to scope");
    if (!scope.AddAxiom(new Term(Term::Type::TypeDeclaration,
        new Term(Term::Type::Symbol, scope.StringToSymbol("topPushAxiom"_M)),
        new Term(Term::Type::Abstraction,
            new Term(Term::Type::Symbol, scope.StringToSymbol("X"_M)),
            new Term(Term::Type::Abstraction,
                new Term(Term::Type::Symbol, scope.StringToSymbol("Stack"_M)),
                new Term(Term::Type::Equivalence,
                    new Term(Term::Type::Application,
                        new Term(Term::Type::Symbol, scope.StringToSymbol("top"_M)),
                        new Term(Term::Type::Application,
                            new Term(Term::Type::Application,
                                new Term(Term::Type::Symbol, scope.StringToSymbol("push"_M)),
                                new Term(Term::Type::Variable, 1)
                            ),
                            new Term(Term::Type::Variable, 0)
                        )
                    ),
                    new Term(Term::Type::Variable, 1)
                )
            )
        )
    ))) MathLib::Panic("Failed to add axiom to scope");
    if (!scope.AddAxiom(new Term(Term::Type::TypeDeclaration,
        new Term(Term::Type::Symbol, scope.StringToSymbol("popNewAxiom"_M)),
        new Term(Term::Type::Equivalence,
            new Term(Term::Type::Application,
                new Term(Term::Type::Symbol, scope.StringToSymbol("pop"_M)),
                new Term(Term::Type::Symbol, scope.StringToSymbol("new"_M))
            ),
            new Term(Term::Type::Symbol, scope.StringToSymbol("new"_M))
        )
    ))) MathLib::Panic("Failed to add axiom to scope");
    LogString(scope.ToString());
    LogChar('\n');
}