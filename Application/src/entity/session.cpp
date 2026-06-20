#include "session.h"

void Session::set_session_version(int sv)
{
    session_version = sv;
}

void Session::set_user_id(int i)
{
    userid = i;
}

void Session::clear_session()
{
    userid = 0;
    session_version = 0;
}