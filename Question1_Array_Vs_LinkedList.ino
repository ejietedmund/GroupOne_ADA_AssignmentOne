#include <Arduino.h>

#define MAX_PLAYLIST_SIZE 10

// ==================== TYPE DEFINITIONS ====================
struct Song {
    char title[50];
    char artist[50];
    int duration; // in seconds
};

// ==================== ARRAY-BASED PLAYLIST ====================
struct ArrayList {
    Song songs[MAX_PLAYLIST_SIZE];
    int size = 0;
};

// ==================== LINKED LIST-BASED PLAYLIST ====================
struct SongNode {
    Song song;
    SongNode* next;
};

struct LinkedList {
    SongNode* head = nullptr;
};

// ==================== FUNCTION PROTOTYPES ====================

// ArrayList functions
void addSong(ArrayList* list, const Song& newSong);
void removeSong(ArrayList* list, int index);
void displayPlaylist(const ArrayList* list);

// LinkedList functions
void addSong(LinkedList* list, const Song& newSong);
void removeSong(LinkedList* list, int index);
void displayPlaylist(const LinkedList* list);
void freeLinkedList(LinkedList* list);

// ==========================================================
// ================= ARRAY LIST FUNCTIONS ===================
// ==========================================================

// Add a song to the end of the array list.
void addSong(ArrayList* list, const Song& newSong) {
    if (list->size >= MAX_PLAYLIST_SIZE) {
        Serial.println("Error: Playlist is full (Array Overflow).");
        return;
    }

    list->songs[list->size] = newSong;
    list->size++;

    Serial.println("Song added successfully (Array).");
}

// Remove a song at given index (0-based)
void removeSong(ArrayList* list, int index) {
    if (index < 0 || index >= list->size) {
        Serial.println("Error: Invalid index (Array).");
        return;
    }

    for (int i = index; i < list->size - 1; i++) {
        list->songs[i] = list->songs[i + 1];
    }

    list->size--;
    Serial.println("Song removed successfully (Array).");
}

// Display all songs
void displayPlaylist(const ArrayList* list) {
    if (list->size == 0) {
        Serial.println("Playlist is empty (Array).");
        return;
    }

    Serial.println("Array Playlist:");
    for (int i = 0; i < list->size; i++) {
        Serial.print(i);
        Serial.print(". ");
        Serial.print(list->songs[i].title);
        Serial.print(" by ");
        Serial.print(list->songs[i].artist);
        Serial.print(" (");
        Serial.print(list->songs[i].duration);
        Serial.println(" sec)");
    }
}

// ==========================================================
// ================= LINKED LIST FUNCTIONS ==================
// ==========================================================

// Add a song to end of linked list
void addSong(LinkedList* list, const Song& newSong) {
    SongNode* newNode = new SongNode;
    newNode->song = newSong;
    newNode->next = nullptr;

    if (list->head == nullptr) {
        list->head = newNode;
    } else {
        SongNode* temp = list->head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    Serial.println("Song added successfully (Linked List).");
}

// Remove a song at given index
void removeSong(LinkedList* list, int index) {
    if (list->head == nullptr || index < 0) {
        Serial.println("Error: Invalid index (Linked List).");
        return;
    }

    if (index == 0) {
        SongNode* temp = list->head;
        list->head = list->head->next;
        delete temp;
        Serial.println("Song removed successfully (Linked List).");
        return;
    }

    SongNode* current = list->head;

    for (int i = 0; i < index - 1 && current->next != nullptr; i++) {
        current = current->next;
    }

    if (current->next == nullptr) {
        Serial.println("Error: Index out of bounds (Linked List).");
        return;
    }

    SongNode* nodeToDelete = current->next;
    current->next = nodeToDelete->next;
    delete nodeToDelete;

    Serial.println("Song removed successfully (Linked List).");
}

// Display linked list
void displayPlaylist(const LinkedList* list) {
    if (list->head == nullptr) {
        Serial.println("Playlist is empty (Linked List).");
        return;
    }

    Serial.println("Linked List Playlist:");
    SongNode* current = list->head;
    int index = 0;

    while (current != nullptr) {
        Serial.print(index++);
        Serial.print(". ");
        Serial.print(current->song.title);
        Serial.print(" by ");
        Serial.print(current->song.artist);
        Serial.print(" (");
        Serial.print(current->song.duration);
        Serial.println(" sec)");

        current = current->next;
    }
}

// Free memory to prevent leaks
void freeLinkedList(LinkedList* list) {
    SongNode* current = list->head;
    while (current != nullptr) {
        SongNode* temp = current;
        current = current->next;
        delete temp;
    }
    list->head = nullptr;
}

// ==========================================================
// ======================== SETUP ===========================
// ==========================================================

void setup() {
    Serial.begin(115200);
    while (!Serial);

    Serial.println("==== ARRAY PLAYLIST TEST ====");
    ArrayList arrayPlaylist;

    addSong(&arrayPlaylist, {"Song A", "Artist 1", 180});
    addSong(&arrayPlaylist, {"Song B", "Artist 2", 220});
    addSong(&arrayPlaylist, {"Song C", "Artist 3", 200});

    displayPlaylist(&arrayPlaylist);

    removeSong(&arrayPlaylist, 1);
    displayPlaylist(&arrayPlaylist);

    Serial.println("\n==== LINKED LIST PLAYLIST TEST ====");
    LinkedList linkedPlaylist;

    addSong(&linkedPlaylist, {"Song X", "Artist X", 195});
    addSong(&linkedPlaylist, {"Song Y", "Artist Y", 210});
    addSong(&linkedPlaylist, {"Song Z", "Artist Z", 205});

    displayPlaylist(&linkedPlaylist);

    removeSong(&linkedPlaylist, 1);
    displayPlaylist(&linkedPlaylist);

    // Free memory
    freeLinkedList(&linkedPlaylist);
}

void loop() {
    // Nothing needed here
}

/*
============================================================
======================= ANALYSIS ===========================
============================================================

1) For a playlist that rarely changes, which implementation 
   is more memory-efficient and why?

The Array-based implementation is more memory-efficient.
Reason:
- Memory is allocated statically (fixed size).
- No extra memory is used for pointers.
- No dynamic allocation overhead.
- No risk of heap fragmentation.

The Linked List uses extra memory for:
- A pointer (SongNode* next) in every node.
- Heap allocation metadata.
Therefore, it consumes more memory per song.

------------------------------------------------------------

2) For a playlist where the user constantly adds and removes
   songs from the middle, which implementation is more 
   time-efficient and why?

The Linked List is more time-efficient.

Reason:
- Array removal requires shifting elements → O(n).
- Linked List removal only changes pointers → O(1) 
  (after traversal).

Thus, frequent middle insertions/removals are faster
with Linked Lists.

------------------------------------------------------------

3) What is the risk of using the Linked List implementation 
   on an ESP32 over a long period?

Risks include:

- Heap fragmentation due to repeated new/delete.
- Memory leaks if nodes are not properly deleted.
- Possible system instability after long runtime.
- Embedded systems like ESP32 have limited RAM.

Therefore, dynamic memory must be carefully managed.

============================================================
END OF QUESTION 1
============================================================
*/