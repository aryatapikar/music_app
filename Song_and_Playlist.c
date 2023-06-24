#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct song
{
	char song_name[100];
	struct node* prev_song;
	struct node* next_song;
}SONG;
typedef struct playlist
{
	char playlist_name[100];
	SONG* first_song;
	SONG* cur_song;
	//struct playlist* prev_playlist;       // will be seen if needed
	struct playlist* next_playlist;
}PLAYLIST;
typedef struct hct_p
{
	PLAYLIST* head;
	PLAYLIST* cur_playlist;
	//PLAYLIST* tail;
}HCT_P;

SONG* getsong(char* songname)
{
	SONG* temp;
	temp = (SONG*)malloc(sizeof(SONG));
	strcpy(temp->song_name, songname);
	temp->next_song = NULL;
	temp->prev_song = NULL;
	return temp;
}
PLAYLIST* getplaylist(char* playlistname)
{
	PLAYLIST* temp;
	temp = (PLAYLIST*)malloc(sizeof(PLAYLIST));
	strcpy(temp->playlist_name, playlistname);
	temp->next_playlist = NULL;
	temp->first_song = NULL;
	temp->cur_song = NULL;
	return temp;
}
void init_playlists(HCT_P* P)
{
	P->head = P->cur_playlist = NULL;
}

PLAYLIST* search_playlist(HCT_P* P, char* targetPlaylist)
{
	PLAYLIST* cur = P->head;
	int position = 1;
	//int found = 0;

	while (cur != NULL)
	{
		if (strcmp(cur->playlist_name, targetPlaylist) == 0)
		{
			printf("Playlist '%s' found at position %d\n", targetPlaylist, position);
			//found = 1;
			return cur;
			//break;
		}
		cur = cur->next_playlist;
		position++;
	}

	//if (!found)
	//{
	printf("Playlist '%s' not found\n", targetPlaylist);
	return NULL;
	//}
}
SONG* search_song(HCT_P* P, char* targetSong, char* targetPlaylist)
{
	PLAYLIST* playlist;
	playlist = search_playlist(P, targetPlaylist);
	if (playlist == NULL)
	{
		return NULL;
	}
	SONG* cur = playlist->first_song;
	int position = 1;
	//int found = 0;

	while (cur != NULL)
	{
		if (strcmp(cur->song_name, targetSong) == 0)
		{
			printf("Song '%s' found at position %d\n", targetSong, position);
			//found = 1;
			return cur;
			//break;
		}
		cur = cur->next_song;
		position++;
	}

	//if (!found)
	//{
	printf("Song '%s' not found in the playlist\n", targetSong);
	return NULL;
	//}
}

void insert_song(HCT_P* P, char* songname, char* targetPlaylist)
{
	PLAYLIST* playlist;
	playlist = search_playlist(P, targetPlaylist);
	if (playlist == NULL)
	{
		return;
	}

	SONG* newsong = getsong(songname);
	SONG* cur = playlist->first_song;
	if (playlist->first_song == NULL)
	{
		playlist->first_song = newsong;
	}
	else
	{
		while (cur->next_song != NULL)
		{
			cur = cur->next_song;
		}
		cur->next_song = newsong;
		newsong->prev_song = cur;
	}
}
void insert_playlist(HCT_P* P, char* playlistname)
{
	PLAYLIST* newplaylist = getplaylist(playlistname);
	PLAYLIST* cur = P->head;
	if (P->head == NULL)
	{
		P->head = newplaylist;
	}
	else
	{
		while (cur->next_playlist != NULL)
		{
			cur = cur->next_playlist;
		}
		cur->next_playlist = newplaylist;
		//newplaylist->prev_playlist = cur;
	}
}

void delete_song(HCT_P* P, char* song, char* targetPlaylist)
{
	PLAYLIST* playlist;
	playlist = search_playlist(P, targetPlaylist);
	if (playlist == NULL)
	{
		return;
	}

	if (playlist->first_song == NULL)
	{
		printf("Song '%s' not found in the playlist\n", song);
		return;
	}
	else if (strcmp(song, playlist->first_song->song_name) == 0)
	{
		SONG* cur = playlist->first_song;
		playlist->first_song = playlist->first_song->next_song;
		playlist->first_song->prev_song = NULL; // to be implemented
		if (playlist->first_song == NULL)
		{
			playlist->cur_song = NULL;
		}
		free(cur);
		return;
	}
	SONG* cur = playlist->first_song->next_song, * prev = playlist->first_song;
	while (cur != NULL)
	{
		if (strcmp(song, cur->song_name) == 0)
		{
			prev->next_song = cur->next_song;
			cur->next_song->prev_song = prev;
			free(cur);
			return;
		}
		cur = cur->next_song;
		prev = prev->next_song;
	}
	printf("Song '%s' not found in the playlist\n", song);
}
void delete_all_songs(HCT_P* P, char* targetPlaylist)
{
	PLAYLIST* playlist;
	playlist = search_playlist(P, targetPlaylist);
	if (playlist == NULL)
	{
		return;
	}

	if (playlist->first_song == NULL)
	{
		return;
	}
	SONG* cur = playlist->first_song;
	while (playlist->first_song != NULL)
	{
		playlist->first_song = playlist->first_song->next_song;
		playlist->first_song->prev_song = NULL; // to be implemented
		free(cur);
	}
	playlist->cur_song = NULL;
}

void delete_playlist(HCT_P* P, char* playlist)
{
	if (P->head == NULL)
	{
		return;
	}
	else if (strcmp(playlist, P->head->playlist_name) == 0)
	{
		PLAYLIST* cur = P->head;
		P->head = P->head->next_playlist;
		if (P->head == NULL)
		{
			P->cur_playlist = NULL;
		}
		free(cur);
		return;
	}
	PLAYLIST* cur = P->head->next_playlist, * prev = P->head;
	while (cur != NULL)
	{
		if (strcmp(playlist, cur->playlist_name) == 0)
		{
			prev->next_playlist = cur->next_playlist;
			delete_all_songs(P, cur);
			free(cur);
			return;
		}
		cur = cur->next_playlist;
		prev = prev->next_playlist;
	}
	printf("Playlist '%s' not found\n", playlist);
}
void delete_all_playlists(HCT_P* P)
{
	if (P->head == NULL)
	{
		return;
	}
	PLAYLIST* cur = P->head;
	while (P->head != NULL)
	{
		P->head = P->head->next_playlist;
		delete_all_songs(P, cur);
		free(cur);
	}
	P->cur_playlist = NULL;
}

void display(HCT_P* P)
{
	PLAYLIST* cur_p = P->head;
	while (cur_p != NULL)
	{
		printf("%s => ", cur_p->playlist_name);
		SONG* cur_s = cur_p->first_song;
		while (cur_s != NULL)
		{
			printf("%s || ", cur_s->song_name);
			cur_s = cur_s->next_song;
		}
		printf("\n\n");
		cur_p = cur_p->next_playlist;
	}
}

int main()
{
	HCT_P s1;
	init_playlists(&s1);
	insert_playlist(&s1, "abc");
	insert_song(&s1, "a1b2c3", "abc");
	insert_song(&s1, "a2b3c4", "abc");
	insert_song(&s1, "a3b4c5", "abc");
	insert_playlist(&s1, "bcd");
	insert_song(&s1, "b4c5d6", "bcd");
	insert_playlist(&s1, "def");
	display(&s1);
	SONG* song1;
	song1 = search_song(&s1, "a2b3c4", "abc");
	PLAYLIST* playlist1;
	playlist1 = search_playlist(&s1, "def");
	delete_song(&s1, "b4c5d6", "bcd");
	delete_playlist(&s1, "bcd");
	delete_all_songs(&s1, "abc");
	display(&s1);
	delete_all_playlists(&s1);
	display(&s1);
	return 0;
}