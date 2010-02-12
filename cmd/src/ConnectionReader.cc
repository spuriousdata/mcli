#include <vector>
#include <stdexcept>
#include <pthread.h>
#include <poll.h>
#include <errno.h>
#include "Connection.h"
#include "MemcacheClient.h"
#include "ConnectionReader.h"

extern int errno;

ConnectionReader::ConnectionReader(std::vector<Connection *>& connections, MemcacheClient *m) :
	connections(connections), m(m)
{
	data_buffer.resize(connections.size());

	pthread_mutex_init(const_cast<pthread_mutex_t*>(&mutex), NULL);

	if (pthread_create(&thread_id, NULL, (void *(*)(void*))ConnectionReader_start, this) != 0)
		throw std::runtime_error("Fatal: Could not start ConnectionReader thread");
}

void ConnectionReader_start(ConnectionReader *instance)
{
	instance->run();
}	

void ConnectionReader::run()
{
	std::vector<struct pollfd> fds;
	struct pollfd pf;
	Connection *c;
	char buffer[4096];
	int ret, bytes_read;

	pf.events = POLLIN;
	pf.revents = 0;

	for (std::vector<Connection *>::const_iterator i = connections.begin(); i != connections.end(); i++) {
		c = *i;
		pf.fd = c->get_sockfd();
		fds.push_back(pf);
	}

	while (true) {
		if ((ret = poll(&fds[0], static_cast<nfds_t>(fds.size()), 0)) == -1) {
			if (errno == EAGAIN) continue;
			throw std::runtime_error("Fatal: Poll() return -1 and errno is not EAGAIN");
		}

		for (int i = 0; i < fds.size(); i++) {
			if ((fds.at(i).revents) & POLLIN) {
				if ((bytes_read = connections.at(i)->recv(buffer,4096)) == 0)
					throw ConnectionClosedException(-1, "Fatal: Connection closed by remote endpoint"); 
				
				acquire_data_lock();
				data_buffer.at(i).insert(data_buffer.at(i).end(), buffer, buffer+bytes_read);
				release_data_lock();
			}
		}

	}
}
