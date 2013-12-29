#include "UnitTest.h"
#include "..\Source\TinymoeExpressionAnalyzer.h"

using namespace tinymoe;

void Tokenize(const string& code, CodeToken::List& tokens)
{
	CodeError::List errors;
	auto codeFile = CodeFile::Parse(code, errors);
	TEST_ASSERT(errors.size() == 0);
	TEST_ASSERT(codeFile->lines.size() == 1);
	tokens = codeFile->lines[0]->tokens;
}

TEST_CASE(TestGrammarStack)
{
	auto stack = make_shared<GrammarStack>();
	auto item = make_shared<GrammarStackItem>();
	item->FillPredefinedSymbols();
	TEST_ASSERT(item->symbols.size() == 24);

	stack->Push(item);
	TEST_ASSERT(stack->stackItems.size() == 1);
	TEST_ASSERT(stack->availableSymbols.size() == item->symbols.size());
	for (auto symbol : item->symbols)
	{
		auto it = stack->availableSymbols.find(symbol->uniqueId);
		TEST_ASSERT(it != stack->availableSymbols.end());
		TEST_ASSERT(it->first == symbol->uniqueId);
		TEST_ASSERT(it->second == symbol);
	}

	TEST_ASSERT(stack->Pop() == item);
	TEST_ASSERT(stack->stackItems.size() == 0);
	TEST_ASSERT(stack->availableSymbols.size() == 0);
}

TEST_CASE(TestParseNameExpression)
{
	auto item = make_shared<GrammarStackItem>();
	item->FillPredefinedSymbols();
	auto stack = make_shared<GrammarStack>();
	stack->Push(item);

	CodeToken::List tokens;
	GrammarStack::ResultList result;

	{
		Tokenize("true", tokens);
		result.clear();
		stack->ParsePrimitive(tokens.begin(), tokens.end(), result);
		TEST_ASSERT(result.size() == 1);

		auto expr = dynamic_pointer_cast<ReferenceExpression>(result[0].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->symbol->uniqueId == "true ");
	}
	{
		Tokenize("false", tokens);
		result.clear();
		stack->ParsePrimitive(tokens.begin(), tokens.end(), result);
		TEST_ASSERT(result.size() == 1);

		auto expr = dynamic_pointer_cast<ReferenceExpression>(result[0].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->symbol->uniqueId == "false ");
	}
	{
		Tokenize("null", tokens);
		result.clear();
		stack->ParsePrimitive(tokens.begin(), tokens.end(), result);
		TEST_ASSERT(result.size() == 1);

		auto expr = dynamic_pointer_cast<ReferenceExpression>(result[0].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->symbol->uniqueId == "null ");
	}
}

TEST_CASE(TestParseTypeExpression)
{
	auto item = make_shared<GrammarStackItem>();
	item->FillPredefinedSymbols();
	auto stack = make_shared<GrammarStack>();
	stack->Push(item);

	CodeToken::List tokens;
	GrammarStack::ResultList result;

	{
		Tokenize("array", tokens);
		result.clear();
		stack->ParseType(tokens.begin(), tokens.end(), result);
		TEST_ASSERT(result.size() == 1);

		auto expr = dynamic_pointer_cast<ReferenceExpression>(result[0].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->symbol->uniqueId == "array ");
	}
	{
		Tokenize("integer", tokens);
		result.clear();
		stack->ParseType(tokens.begin(), tokens.end(), result);
		TEST_ASSERT(result.size() == 1);

		auto expr = dynamic_pointer_cast<ReferenceExpression>(result[0].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->symbol->uniqueId == "integer ");
	}
	{
		Tokenize("float", tokens);
		result.clear();
		stack->ParseType(tokens.begin(), tokens.end(), result);
		TEST_ASSERT(result.size() == 1);

		auto expr = dynamic_pointer_cast<ReferenceExpression>(result[0].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->symbol->uniqueId == "float ");
	}
	{
		Tokenize("string", tokens);
		result.clear();
		stack->ParseType(tokens.begin(), tokens.end(), result);
		TEST_ASSERT(result.size() == 1);

		auto expr = dynamic_pointer_cast<ReferenceExpression>(result[0].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->symbol->uniqueId == "string ");
	}
	{
		Tokenize("symbol", tokens);
		result.clear();
		stack->ParseType(tokens.begin(), tokens.end(), result);
		TEST_ASSERT(result.size() == 1);

		auto expr = dynamic_pointer_cast<ReferenceExpression>(result[0].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->symbol->uniqueId == "symbol ");
	}
}

TEST_CASE(TestParseArgumentExpression)
{
	auto item = make_shared<GrammarStackItem>();
	item->FillPredefinedSymbols();
	auto stack = make_shared<GrammarStack>();
	stack->Push(item);

	CodeToken::List tokens;
	GrammarStack::ResultList result;

	Tokenize("true end", tokens);
	result.clear();
	stack->ParseArgument(tokens.begin(), tokens.end(), result);
	TEST_ASSERT(result.size() == 2);
	{
		auto expr = dynamic_pointer_cast<ArgumentExpression>(result[0].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->tokens.size() == 1);
		TEST_ASSERT(expr->tokens[0].value == "true");
	}
	{
		auto expr = dynamic_pointer_cast<ArgumentExpression>(result[1].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->tokens.size() == 2);
		TEST_ASSERT(expr->tokens[0].value == "true");
		TEST_ASSERT(expr->tokens[1].value == "end");
	}
}

TEST_CASE(TestParseAssignableExpression)
{
	auto item = make_shared<GrammarStackItem>();
	item->FillPredefinedSymbols();
	auto stack = make_shared<GrammarStack>();
	stack->Push(item);

	CodeToken::List tokens;
	GrammarStack::ResultList result;

	Tokenize("true end", tokens);
	result.clear();
	stack->ParseAssignable(tokens.begin(), tokens.end(), result);
	TEST_ASSERT(result.size() == 2);
	{
		auto expr = dynamic_pointer_cast<ReferenceExpression>(result[0].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->symbol->uniqueId == "true ");
	}
	{
		auto expr = dynamic_pointer_cast<ArgumentExpression>(result[1].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->tokens.size() == 2);
		TEST_ASSERT(expr->tokens[0].value == "true");
		TEST_ASSERT(expr->tokens[1].value == "end");
	}
}

TEST_CASE(TestParsePrimitiveExpression)
{
}

TEST_CASE(TestParseUnaryExpression)
{
}

TEST_CASE(TestParseBinaryExpression)
{
}

TEST_CASE(TestParseListExpression)
{
}

TEST_CASE(TestParseInvokeExpression)
{
}

TEST_CASE(TestParseStatement)
{
}