#include "my_rwlock.h"

void my_rwlock_init(my_rwlock_t *rw) {
    pthread_mutex_init(&rw->mutex, NULL);
    pthread_cond_init(&rw->can_read, NULL);
    pthread_cond_init(&rw->can_write, NULL);
    rw->readers = 0;
    rw->waiting_readers = 0;
    rw->waiting_writers = 0;
    rw->writer = 0;
}

void my_rwlock_destroy(my_rwlock_t *rw) {
    pthread_mutex_destroy(&rw->mutex);
    pthread_cond_destroy(&rw->can_read);
    pthread_cond_destroy(&rw->can_write);
}

void my_rwlock_rdlock(my_rwlock_t *rw) {
    pthread_mutex_lock(&rw->mutex);
    rw->waiting_readers++;
    while (rw->writer || rw->waiting_writers > 0)
        pthread_cond_wait(&rw->can_read, &rw->mutex);
    rw->waiting_readers--;
    rw->readers++;
    pthread_mutex_unlock(&rw->mutex);
}

void my_rwlock_wrlock(my_rwlock_t *rw) {
    pthread_mutex_lock(&rw->mutex);
    rw->waiting_writers++;
    while (rw->writer || rw->readers > 0)
        pthread_cond_wait(&rw->can_write, &rw->mutex);
    rw->waiting_writers--;
    rw->writer = 1;
    pthread_mutex_unlock(&rw->mutex);
}

void my_rwlock_unlock(my_rwlock_t *rw) {
    pthread_mutex_lock(&rw->mutex);
    if (rw->writer)
        rw->writer = 0;
    else
        rw->readers--;

    if (rw->readers == 0 && rw->waiting_writers > 0)
        pthread_cond_signal(&rw->can_write);
    else if (rw->readers == 0 && rw->waiting_readers > 0)
        pthread_cond_broadcast(&rw->can_read);

    pthread_mutex_unlock(&rw->mutex);
}
