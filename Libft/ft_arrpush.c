char    **ft_arpush(char **ar, char *data)
{
    char            **new_ar;
    size_t          size;
    unsigned int    i;

    i = 0;
    size = ft_count_arr((void **)ar) + 1;
    if ((new_ar = malloc(sizeof(char *) * (size + 1))) == NULL)
        return (NULL);
    while (i < size - 1)
    {
        new_ar[i] = ft_strdup(ar[i]);
        i++;
    }
    new_ar[i] = data;
    new_ar[i + 1] = NULL;
    return (new_ar);
}
