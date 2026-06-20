#pragma once

class Session
{
    private:
        int userid;
        int session_version;

    public:
        //setter
        void set_session_version(int sv);
        void set_user_id(int i);

        //getter
        int get_session_version(){return session_version;}
        int get_user_id(){return userid;}
        void clear_session();
};