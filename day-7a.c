typedef struct tree {
	struct tree *parent;
	struct tree *next;
	uint8_t name[0x100];
	enum { directory, file } type;
	union
	{
		struct { struct tree *children; } directory;
		struct { uint64_t size; } file;
	};
} tree;

tree * tree_alloc(uint8_t *name)
{
	uint64_t name_len = strlen(name);
	if (name_len >= 0x100) return (void*) 0;

	tree *t = mmap((void*) 0, sizeof(tree), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	memcpy(name, t->name, name_len + 1);

	return t;
}

tree * tree_alloc_directory(uint8_t *name)
{
	tree *t = tree_alloc(name);
	if (t) t->type = directory;
	return t;
}

tree * tree_alloc_file(uint8_t *name, uint64_t size)
{
	tree *t = tree_alloc(name);
	if (t)
	{
		t->type = file;
		t->file.size = size;
	}
	return t;
}

void tree_free(tree *t)
{
	if (t->next) tree_free(t->next);
	if (t->type == directory && t->directory.children) tree_free(t->directory.children);
	munmap(t, sizeof(tree));
}

tree * tree_get_node(tree *t, uint8_t *name)
{
	if (t->type != directory) return (void*) 0;
	if (strcmp(name, (uint8_t*) "..")) return t->parent;

	for (t = t->directory.children; t; t = t->next)
	{
		if (strcmp(name, t->name)) return t;
	}

	return (void*) 0;
}

int32_t tree_add_node(tree *t, tree *node)
{
	node->parent = t;
	if (t->type != directory) return 1;
	if (!t->directory.children) t->directory.children = node;
	else
	{
		t = t->directory.children;
		while (t->next) t = t->next;
		t->next = node;
	}

	return 0;
}

uint64_t recursive_size(tree *t)
{
	if (t->type == file) return t->file.size;
	uint64_t children_size = 0;
	for (tree *child = t->directory.children; child; child = child->next)
	{
		children_size += recursive_size(child);
	}
	return children_size;
}

uint64_t day_7(tree *t)
{
	if (t->type != directory) return 0;
	uint64_t size = recursive_size(t);
	if (size > 100000) size = 0;
	for (tree *child = t->directory.children; child; child = child->next) size += day_7(child);
	return size;
}

int32_t print_tree(tree *t, uint64_t indent)
{
	uint8_t buf[0x100];

	uint64_t name_len = strlen(t->name);
	if (indent + 2 + name_len >= sizeof(buf)) return 1;

	for (uint64_t i = 0; i < indent; ++i) buf[i] = ' ';
	buf[indent] = '-';
	buf[indent+1] = ' ';
	memcpy(t->name, buf + indent + 2, name_len);
	buf[indent + 2 + name_len] = ' ';

	uint64_t size = recursive_size(t);
	uint64_t len = format_dec(size, buf + indent + 2 + name_len + 1, sizeof(buf) - (indent + 2 + name_len + 1));
	if (!len) return 1;
	buf[indent + 2 + name_len + 1 + len] = '\n';

	write(1, buf, indent + 2 + name_len + 1 + len + 1);

	if (t->type == directory && t->directory.children)
	{
		int32_t err = print_tree(t->directory.children, indent + 1);
		if (err) return err;
	}
	if (t->next)
	{
		int32_t err = print_tree(t->next, indent);
		if (err) return err;
	}

	return 0;
}

void init()
{
	tree *root = tree_alloc_directory((uint8_t*) "/");
	if (!root) exit(1);
	tree *work = root;

	uint8_t buf[0x400];
	linebuf *f = linebuf_open(0);
	while (!linebuf_eof(f) && !linebuf_error(f))
	{
		uint64_t len = linebuf_read(f, buf, sizeof(buf));
		if (len <= 1) continue;

		if(*buf == '$')
		{
			if (buf[2] == 'c')
			{
				uint8_t *name = buf + 5;
				if (*name == '/') work = root;
				else work = tree_get_node(work, name);
				if (!work) exit(1);
			}
		}
		else
		{
			tree *node;
			if (*buf == 'd')
			{
				uint8_t *name = buf + 4;
				node = tree_alloc_directory(name);
			}
			else
			{
				uint8_t *name = findchar(' ', buf);
				if (!name) exit(1);
				*(name++) = 0;
				int32_t size;
				int32_t err = parse_dec(buf, &size);
				if (err) exit(1);
				node = tree_alloc_file(name, size);
			}
			tree_add_node(work, node);
		}
	}
	if (linebuf_error(f)) exit(1);
	linebuf_free(f);

	print_tree(root, 0);

	uint64_t size = day_7(root);
	uint64_t len = format_dec(size, buf, sizeof(buf));
	if (!len) exit(1);
	buf[len-1] = '\n';
	write(1, buf, len);

	exit(0);
}
