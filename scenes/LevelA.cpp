#include "LevelA.h"
#include "../Utility.h"

#define LEVEL_WIDTH 20
#define LEVEL_HEIGHT 20
#define ENEMY_COUNT 1

unsigned int LEVEL_A_DATA[] =
{
    121,122,122,122,122,122,122,122,122,122,122,122,122,122,4,143,0,0,0,0,
    121,122,122,122,122,104,122,122,122,122,122,104,4,142,143,0,0,0,0,0,
    121,104,104,122,122,122,104,104,104,122,122,4,143,69,0,0,0,0,0,0,
    121,122,122,122,104,122,122,104,122,4,142,143,0,89,0,0,0,0,0,0,
    121,122,104,122,104,104,122,4,142,143,0,0,0,89,0,0,0,0,0,21,
    121,104,122,122,104,104,4,143,0,0,0,0,0,89,0,0,0,0,21,25,
    121,122,104,104,4,142,143,0,0,0,0,0,0,89,0,0,21,22,25,122,
    121,104,122,4,143,0,0,0,0,0,0,0,0,89,0,0,141,5,122,122,
    121,122,4,143,0,0,0,0,0,0,0,0,0,89,0,0,0,141,5,122,
    121,122,123,0,0,0,0,0,0,0,0,0,0,89,0,0,0,0,121,104,
    121,4,143,0,0,0,0,0,0,0,0,0,0,89,0,0,0,0,121,122,
    121,143,0,0,0,0,0,0,0,0,0,0,0,109,0,108,0,0,121,104,
    120,0,0,0,0,0,0,0,0,0,0,0,48,49,49,50,90,92,121,122,
    120,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,121,104,
    120,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,121,122,
    120,0,0,0,60,0,0,0,0,0,60,0,0,0,0,0,0,0,141,5,
    121,62,62,62,122,62,62,62,62,62,123,0,0,0,0,0,0,0,0,121,
    121,104,104,122,122,122,104,104,122,122,123,0,0,0,0,0,0,0,0,141,
    121,122,122,104,122,122,122,122,104,122,123,0,0,0,0,0,0,0,0,0,
    121,122,104,122,122,122,104,122,104,122,123,0,0,0,0,0,0,0,0,0

};

LevelA::~LevelA()
{
    delete [] m_state.enemies;
    delete    m_state.player;
    delete    m_state.map;
    Mix_FreeChunk(m_state.jump_sfx);
    Mix_FreeMusic(m_state.bgm);
    Mix_FreeChunk(m_state.pop_sfx);
}

void LevelA::initialise()
{
    GLuint map_texture_id = Utility::load_texture("assets/images/tilemap_packed.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL_A_DATA, map_texture_id, 1.0f, 20, 9);
    
    // ————— PLAYER SET-UP ————— //
    // Existing
    m_state.player = new Entity();
    m_state.player->set_entity_type(PLAYER);
    m_state.player->set_position(glm::vec3(2.0f, -15.0f, 0.0f));
    m_state.player->set_init_pos(glm::vec3(2.0f, -15.0f, 0.0f));
    m_state.player->set_movement(glm::vec3(0.0f));
    m_state.player->set_speed(2.5f);
    m_state.player->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    m_state.player->m_texture_id = Utility::load_texture("assets/images/tilemap-player.png");
    
    // Animation
    m_state.player->m_walking[m_state.player->RIGHT] = new int[2] { 0, 1 };
    m_state.player->m_walking[m_state.player->LEFT] = new int[2] { 2, 3 };
    
    m_state.player->m_animation_indices = m_state.player->m_walking[m_state.player->RIGHT];
    m_state.player->m_animation_frames = 2;
    m_state.player->m_animation_index = 0;
    m_state.player->m_animation_time = 0.0f;
    m_state.player->m_animation_cols = 4;
    m_state.player->m_animation_rows = 1;
    m_state.player->set_height(0.9f);
    m_state.player->set_width(0.9f);
    
    // Jumping
    m_state.player->m_jumping_power = 8.0f;
    
    // ————— ENEMY SET-UP ————— //
    //BUG
    GLuint bug_texture_id = Utility::load_texture("assets/images/tilemap-bug.png");
    GLuint wasp_texture_id = Utility::load_texture("assets/images/tilemap-characters_packed.png");

    m_state.enemies = new Entity[ENEMY_COUNT];
    
    m_state.enemies[0].set_entity_type(ENEMY);
    m_state.enemies[0].set_ai_type(BUG);
    m_state.enemies[0].set_ai_state(PATROL);
    m_state.enemies[0].m_texture_id = bug_texture_id;
    m_state.enemies[0].set_position(glm::vec3(7.0f, -15.0f, 0.0f));
    m_state.enemies[0].set_init_pos(glm::vec3(7.0f, -15.0f, 0.0f));
    m_state.enemies[0].set_movement(glm::vec3(-1.0f, 0.0f, 0.0f));
    m_state.enemies[0].set_speed(0.5f);
    m_state.enemies[0].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    
    m_state.enemies[0].m_walking[m_state.enemies[0].LEFT] = new int[2] { 0, 1 };
    m_state.enemies[0].m_walking[m_state.enemies[0].RIGHT] = new int[2] { 2, 3 };
                 
    m_state.enemies[0].m_animation_indices = m_state.enemies[0].m_walking[m_state.enemies[0].LEFT];
    m_state.enemies[0].m_animation_frames = 2;
    m_state.enemies[0].m_animation_index = 0;
    m_state.enemies[0].m_animation_time = 0.0f;
    m_state.enemies[0].m_animation_cols = 4;
    m_state.enemies[0].m_animation_rows = 1;
    m_state.enemies[0].set_height(1.0f);
    m_state.enemies[0].set_width(1.0f);
    
    // ————— AUDIO SET-UP ————— //
    Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 4096);

    m_state.bgm = Mix_LoadMUS("assets/audio/tabun.wav");
    Mix_PlayMusic(m_state.bgm, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 32.0f);

    m_state.jump_sfx = Mix_LoadWAV("assets/audio/jump1.wav");
    Mix_VolumeChunk(m_state.jump_sfx, MIX_MAX_VOLUME / 32.0f);

    m_state.pop_sfx = Mix_LoadWAV("assets/audio/bubble1.wav");
    Mix_VolumeChunk(m_state.pop_sfx, MIX_MAX_VOLUME / 32.0f);
}

void LevelA::update(float delta_time)
{
    m_state.player->update(delta_time, m_state.player, m_state.enemies, ENEMY_COUNT, m_state.map, m_state.pop_sfx);
    
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].update(delta_time, m_state.player, 0, 0, m_state.map, m_state.pop_sfx);
    }
}


void LevelA::render(ShaderProgram *program)
{
    m_state.map->render(program);
    m_state.player->render(program);
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].render(program);
    }
}
