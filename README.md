# town-cat

# naming conventions

```
typedef struct thing
{
    
} thing_t;

INSTANCE / REFERENCING:
thing_t* create_thing();
void destroy_thing(thing_t* thing);

STATIC:
static thing_t* thing;
void init_thing();
void destroy_thing();
```

# refarctoring!!!

```
entity:
    rect, bitmap*, animation timer
    rect, bitmap, animation, timer
    (entity*) new_entity(rect, bitmap*)
    (entity*) new_entity_anim(rect, animation)
    destroy_entity(entity*)

animation:
    u8, u8, bitmap**
    current_frame, total_frames, bitmaps
    (timer*) new_timer(wait, type)
    start_timer(timer*)
    stop_timer(timer*)
    kill_timer(timer*)
    (bool) tick_timer(timer*, fps)

world:
    world_id, rect*, bitmap, u8
    id, bounds, bitmap, total_bounds
```

