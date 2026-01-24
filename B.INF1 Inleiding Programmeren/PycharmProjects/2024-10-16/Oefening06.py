def list2dict(l1,l2):
    if len(l1)!=len(l2):
        return
    uitkomst = {}
    for i in range(len(l1)):
        uitkomst[l1[i]] = l2[i]
    return uitkomst
print(list2dict(['Ten', 'Twenty', 'Thirty'],[10, 20, 30]))
