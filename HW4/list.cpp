void listAll(const MenuItem* m, string path) // two-parameter overload
{
    if(m->name() != "")
    {
        cout << path + m->name() << endl;
        path = path + m->name() + "/";
        if(m->menuItems() == NULL)
            return;
        vector<MenuItem*>::const_iterator iter = m->menuItems()->begin();
        for(;iter != m->menuItems()->end(); iter++)
            listAll(*iter, path);
    }
    else
    {
        if(m->menuItems() == NULL)
            return;
        vector<MenuItem*>::const_iterator iter = m->menuItems()->begin();
        for(;iter != m->menuItems()->end(); iter++)
            listAll(*iter, path + m->name());
    }
}