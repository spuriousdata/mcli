#ifndef __CONNECTIONREADER_H__
#define __CONNECTIONREADER_H__

#include <vector>
#include <pthread.h>

class Connection;
class MemcacheClient;

class ConnectionReader
{
	public:
		ConnectionReader(std::vector<Connection*>& connections, MemcacheClient *m);
		~ConnectionReader() { pthread_mutex_destroy(const_cast<pthread_mutex_t*>(&mutex)); }
		int acquire_data_lock() { return pthread_mutex_lock(const_cast<pthread_mutex_t*>(&mutex)); }
		int release_data_lock() { return pthread_mutex_unlock(const_cast<pthread_mutex_t*>(&mutex)); }
		std::vector< std::vector<char> >& get_data() { return data_buffer; }
		void run();

		
	private:
		std::vector<Connection *> connections;
		pthread_t thread_id;
		volatile pthread_mutex_t mutex;
		MemcacheClient *m;
		std::vector< std::vector<char> > data_buffer;

};

void ConnectionReader_start(ConnectionReader *instance);
#endif
