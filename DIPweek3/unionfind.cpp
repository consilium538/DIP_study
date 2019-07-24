class unionfind
{
private:
    unionfind *parent;
    int label;
public:
    unionfind(int label): label(label) { parent = nullptr; }
    unionfind* findroot();
    void merge(unionfind* suspect);
};

unionfind* unionfind::findroot()
{
    unionfind *cur = parent;
    while(cur->parent == nullptr) cur = cur->parent;
    return cur;
}

void unionfind::merge(unionfind* suspect)
{
    unionfind* a = this->findroot();
    unionfind* b = suspect->findroot();
    return;
}