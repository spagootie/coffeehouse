CREATE TABLE users (
    user_id INT NOT NULL AUTO_INCREMENT,
    username VARCHAR(50) NOT NULL UNIQUE,
    display_name VARCHAR(100),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (user_id)
) ENGINE=InnoDB;

CREATE TABLE conversations (
    conversation_id INT NOT NULL AUTO_INCREMENT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (conversation_id)
) ENGINE=InnoDB;

CREATE TABLE conversation_participants (
    conversation_id INT NOT NULL,
    user_id INT NOT NULL,
    joined_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (conversation_id, user_id),
    CONSTRAINT fk_cp_conversation
        FOREIGN KEY (conversation_id)
        REFERENCES conversations(conversation_id),
    CONSTRAINT fk_cp_user
        FOREIGN KEY (user_id)
        REFERENCES users(user_id)
) ENGINE=InnoDB;

CREATE TABLE messages (
    message_id INT NOT NULL AUTO_INCREMENT,
    conversation_id INT NOT NULL,
    sender_id INT NOT NULL,
    content TEXT NOT NULL,
    sent_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (message_id),
    CONSTRAINT fk_msg_conversation
        FOREIGN KEY (conversation_id)
        REFERENCES conversations(conversation_id),
    CONSTRAINT fk_msg_sender
        FOREIGN KEY (sender_id)
        REFERENCES users(user_id)
) ENGINE=InnoDB;
