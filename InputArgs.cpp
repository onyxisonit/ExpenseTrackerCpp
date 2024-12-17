#include "InputArgs.h"

InputArgs::InputArgs(std::string_view line){
    enum class State{
        ReadyForText,
        TextBegin,

    } state = State::ReadyForText;

    std::stringstream ss;
    for (char c: line){
        switch (state){
            case State::ReadyForText:
                switch(c){
                    case ' ': //ignore white spaces
                        break;
                    default:
                        ss << c;
                        state = State::TextBegin; //change state to TextBegin
                        break;
                } break;
            case State::TextBegin:
                switch(c){
                    case ' ': //end of argument, get out
                        m_args.push_back(ss.str()); //save white space to vector
                        ss.str(""); //clear the string stream so it is ready for new text
                        state = State::ReadyForText;
                        break;
                    default:
                        ss << c;
                        break;
                } break;
        }
    }
}