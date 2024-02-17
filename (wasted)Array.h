typedef struct ARRAY {
	void *arr;
	long int size, capacity;
} Array;

static Array *__ARRAY_INIT(unsigned int type_size, unsigned int array_capacity) {
	Array *array = (Array*)malloc(sizeof(Array));
	assert(array);

	array->arr = malloc(type_size * array_capacity);
	assert(array->arr);

	array->size = 0;
	array->capacity = array_capacity;
	return array;
}

static void __ARRAY_INSERT(Array *array, unsigned int type_size, void *element) {
	assert(array);

	if(array->size == array->capacity) { 
		array->capacity *= 2;
		array->arr = realloc(array->arr, type_size * array->capacity);
	}
	assert(array->arr);

	unsigned char *src = (unsigned char*)element, *dst = ((unsigned char*)array->arr) + type_size * (array->size++);
	for(int i = 0; i < type_size; ++i) { dst[i] = src[i]; }
}
#define Array_Init(type, capacity) (__ARRAY_INIT(sizeof(type), (capacity)))
#define Array_Insert(type, array, element) (__ARRAY_INSERT((array), sizeof(type), (element)))
#define Array_Get(type, array, idx) ((type)(((type*)((array)->arr))[(idx)]))