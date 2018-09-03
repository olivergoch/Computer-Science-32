#include <iostream>
#include <string>
#include <stack>
#include <cassert>
using namespace std;


int evaluate(string infix, const bool values[], string& postfix, bool& result)
{
    //INFIX TO POSTFIX
    postfix = "";
    int parens = 0;
    int normoperators = 0;
    int operands = 0;
    stack<char> operatorStack;
    for(int i = 0; i < infix.size(); i++)
    {
        switch(infix[i])
        {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                postfix += infix[i];
                operands++;
                break;
            case '(':
                operatorStack.push(infix[i]);
                parens++;
                break;
            case ')':
                while(operatorStack.top() != '(')
                {
                    postfix += operatorStack.top();
                    operatorStack.pop();
                }
                operatorStack.pop();
                parens--;
                break;
            case '!':
                while(!operatorStack.empty() && operatorStack.top() != '(' && operatorStack.top() != '&'
                      && operatorStack.top() != '|')
                {
                    postfix += operatorStack.top();
                    operatorStack.pop();
                }
                operatorStack.push(infix[i]);
                break;
            case '&':
                while(!operatorStack.empty() && operatorStack.top() != '(' && operatorStack.top() != '|')
                {
                    postfix += operatorStack.top();
                    operatorStack.pop();
                }
                operatorStack.push(infix[i]);
                if(i-1 < 0)
                    return 1;
                for(int j = i-1; j >= 0; j--)
                {
                    if(isdigit(infix[j]))
                        goto allWell;
                    if(!isdigit(infix[j]) && infix[j] != ' ')
                        return 1;
                }
            allWell:
                normoperators++;
                break;
            case '|':
                while(!operatorStack.empty() && operatorStack.top() != '(')
                {
                    postfix += operatorStack.top();
                    operatorStack.pop();
                }
                operatorStack.push(infix[i]);
                if(i-1 < 0)
                    return 1;
                for(int j = i-1; j >= 0; j--)
                {
                    if(isdigit(infix[j]))
                        goto allGood;
                    if(!isdigit(infix[j]) && infix[j] != ' ')
                        return 1;
                }
            allGood:
                normoperators++;
                break;
            case ' ':
                break;
            default:
                return 1;
                break;
        }
    }
    if(parens != 0 || operands == 0 || operands-normoperators != 1)
        return 1;
    while(!operatorStack.empty())
    {
        postfix += operatorStack.top();
        operatorStack.pop();
    }
    //EVALUATE POSTFIX
    stack<bool> operandStack;
    for(int i = 0; i < postfix.size(); i++)
    {
        if(isdigit(postfix[i]))
        {
            int place = postfix[i] - 48;
            operandStack.push(values[place]);
        }
        else if(postfix[i] == '!')
        {
            bool opposite = operandStack.top();
            operandStack.pop();
            opposite = !opposite;
            operandStack.push(opposite);
        }
        else if(postfix[i] == '&')
        {
            bool operand2 = operandStack.top();
            operandStack.pop();
            bool operand1 = operandStack.top();
            operandStack.pop();
            bool result = operand1 == operand2;
            operandStack.push(result);
        }
        else if(postfix[i] == '|')
        {
            bool operand2 = operandStack.top();
            operandStack.pop();
            bool operand1 = operandStack.top();
            operandStack.pop();
            if(operand1 || operand2)
                result = true;
            else
                result = false;
            operandStack.push(result);
        }
    }
    result = operandStack.top();
    return 0;
}