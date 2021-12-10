import axios from "axios";

const api = axios.create({
  baseURL: "http://flaviab.pythonanywhere.com",
});

export default api;