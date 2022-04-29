# include <iostream>
# include <vector>



//===------------------------------------------------==//
// Lexer
//===------------------------------------------------==//

// The lexer returns tokens [0-255] if it is an unknown character, otherwise one
// of these for know things.
enum Token {
    tok_eof = -1,

    // commands
    tok_def = -2, tok_extern = -3,

    // primary
    tok_identifier = -4, tok_number = -5
};

static std::string IdentifierStr;   // Filled in if tok_identifier
static double NumVal;               // Filled in if tok_number

/// gettok - Return the next token from standard input.
static int gettok() {
    static int LastChar = ' ';

    // Skip any whitespace.
    while (isspace(LastChar))
        LastChar = getchar();
    
    if (isalpha(LastChar)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
        IdentifierStr = LastChar;
        while (isalnum((LastChar = getchar())))
            IdentifierStr += LastChar;
        
        if (IdentifierStr == "def") return tok_def;
        if (IdentifierStr == "extern") return tok_extern;
        return tok_identifier;
    }

    if (isdigit(LastChar) || LastChar == '.') {     // Number: [0-9.]+
        std::string NumStr;
        do {
            NumStr += LastChar;
            LastChar = getchar();
        } while (isdigit(LastChar) || LastChar == '.');

        NumVal = strtod(NumStr.c_str(), 0);
        return tok_number;
    }

    if (LastChar == '#') {
        // Comment until end of line.
        do LastChar = getchar();
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF)
            return gettok();
    }

    // Check for end of file.   Don't eat the EOF.
    if (LastChar == EOF)
        return tok_eof;
    
    // Otherwise, just return the character as its ascii value.
    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}





//===------------------------------------------------==//
// Abstract Syntax Tree (aka Parse Tree)
//===------------------------------------------------==//

/// ExprAST - Base class for all expression nodes.
class ExprAST {
public:
    virtual ~ExprAST() {}
};

/// NumberExprAST - Expression class for numeric literals like "1.0".
class NumberExprAST : public ExprAST {
    double Val;
public:
    NumberExprAST(double val) : Val(val) {}
};

/// VariableExprAST - Expression class for referencing a variable, like "a".
class VariableExprAST : public ExprAST {
    std::string Name;
public:
    VariableExprAST(const std::string &name) : Name(name) {}
};

/// BinaryExprAST - Expression class for a binary operator.
class BinaryExprAST : public ExprAST {
    char Op;
    ExprAST *LHS, *RHS;
public:
    BinaryExprAST(char op, ExprAST *lhs, ExprAST *rhs) 
        : Op(op), LHS(lhs), RHS(rhs) {}
};

/// CallExprAST - Expression class for function calls.
class CallExprAST : public ExprAST {
    std::string Callee;
    std::vector<ExprAST*> Args;
public:
    CallExprAST(const std::string &callee, std::vector<ExprAST*> &args) 
        : Callee(callee), Args(args) {}
};


/// PrototypeAST - This class represents the "prototype" for a function,
/// which captures its name. and its argument names (thus implicitly the number
/// of arguments the function takes).
class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args;
public:
    PrototypeAST(const std::string &name, const std::vector<std::string> &args) 
        : Name(name), Args(args) {}
};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST {
    PrototypeAST * Proto;
    ExprAST * Body;
public:
    FunctionAST(PrototypeAST * proto, ExprAST * body) 
        : Proto(proto), Body(body) {}
};


ExprAST * X = new VariableExprAST("x");
ExprAST * Y = new VariableExprAST("y");
ExprAST * Result = new BinaryExprAST('+', X, Y);


/// CurTok/getNextToken - Provide a simple token buffer. CurTok is the current
/// token the parser is looking at.   getNextToken reads another token from the
/// lexer and updates CurTok with its results.
static int CurTok;
static int getNextToken() {
    return CurTok = gettok();
}

/// Error * - These are little helper functions for error handing.
ExprAST * Error(const char * Str) { fprintf(stderr, "Error: %s\n", Str); return 0; }
PrototypeAST * ErrorP(const char * Str) { Error(Str); return 0; }
FunctionAST * ErrorF(const char * Str) { Error(Str);  return 0; }


/// numberexpr ::= number
static ExprAST * ParseNumberExpr () {
    ExprAST * Result = new NumberExprAST(NumVal);
    getNextToken(); // consume the number
    return Result;
}

/// parenexpr ::= '(' expression ')'
static ExprAST * ParseParenExpr () {
    getNextToken(); // eat (.
    ExprAST * V = Pars
}
