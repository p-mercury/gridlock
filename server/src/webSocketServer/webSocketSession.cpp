#include "webSocketSession.hpp"

WebSocketSession::WebSocketSession(
    boost::asio::ip::tcp::socket&& socket_)
    :ws_(std::move(socket_)){


}

// Get on the correct executor
void WebSocketSession::run(){

    // We need to be executing within a strand to perform async operations
    // on the I/O objects in this session. Although not strictly necessary
    // for single-threaded contexts, this example code is written to be
    // thread-safe by default.
    boost::asio::dispatch(ws_.get_executor(),
        boost::beast::bind_front_handler(
            &WebSocketSession::on_run,
            shared_from_this()));
}

// Start the asynchronous operation
void WebSocketSession::on_run(){

    // Set suggested timeout settings for the websocket
    ws_.set_option(
        boost::beast::websocket::stream_base::timeout::suggested(
            boost::beast::role_type::server));

    // Set a decorator to change the Server of the handshake
    ws_.set_option(boost::beast::websocket::stream_base::decorator(
        [](boost::beast::websocket::response_type& res)
        {
            res.set(boost::beast::http::field::server,
                std::string(BOOST_BEAST_VERSION_STRING) +
                    " websocket-server-async");
        }));
    // Accept the websocket handshake
    ws_.async_accept(
        boost::beast::bind_front_handler(
            &WebSocketSession::on_accept,
            shared_from_this()));
}

void WebSocketSession::on_accept(
    boost::beast::error_code ec){

    if(ec)
        return fail(ec, "accept");

    // Read a message
    do_read();
}

void WebSocketSession::do_read(){

    // Read a message into our buffer
    ws_.async_read(
        buffer_,
        boost::beast::bind_front_handler(
            &WebSocketSession::on_read,
            shared_from_this()));
}

void WebSocketSession::on_read(
    boost::beast::error_code ec,
    std::size_t bytes_transferred){

    boost::ignore_unused(bytes_transferred);

    // This indicates that the session was closed
    if(ec == boost::beast::websocket::error::closed)
        return;

    if(ec)
        fail(ec, "read");

    // Echo the message
    ws_.text(ws_.got_text());
    ws_.async_write(
        buffer_.data(),
        boost::beast::bind_front_handler(
            &WebSocketSession::on_write,
            shared_from_this()));
}

void WebSocketSession::on_write(
    boost::beast::error_code ec,
    std::size_t bytes_transferred){

    boost::ignore_unused(bytes_transferred);

    if(ec)
        return fail(ec, "write");

    // Clear the buffer
    buffer_.consume(buffer_.size());

    // Do another read
    do_read();
}

void WebSocketSession::fail(
    boost::beast::error_code ec,
    char const* what){

    // Don't report these
    if( ec == boost::asio::error::operation_aborted ||
        ec == boost::beast::websocket::error::closed)
        return;

    std::cerr << what << ": " << ec.message() << "\n";
}